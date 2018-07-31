#include "VirtualMachine.h"
#include "Machine.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits.h>
#include <string.h>
using namespace std;

const TVMMemoryPoolID VM_MEMORY_POOL_ID_SYSTEM = 0; // if you have it set ID 0.

typedef struct {
	TVMMutexID mutexID;
	bool isLocked;
	TVMThreadID threadID; //what ID owns it.
	
}Mutex;

typedef struct {
	TVMMemorySize length;
	char* basePtr;
	bool freed;
} MemoryBlock;

typedef struct {
	TVMMemoryPoolID id;
	std::vector<MemoryBlock> blocks;
	TVMMemorySize remainingSpace;
}MemoryPool;

typedef struct {//all info dealing with the thread
	volatile int fileDescriptor;
	TVMMemorySize memorysize;
	TVMThreadPriority priority;
	TVMThreadID tidNum;
	void* stack;
	SMachineContext threadInfo;//pass into machineCreate and it fills this with appropriate info
	TVMThreadState state;
	TVMThreadEntry entry;
	TVMTick tickCount;
	volatile TVMTick sleepCount;
	Mutex mutex;
}TCB;

typedef struct {
	TVMThreadEntry entry;
	void* param;
} skeleStruct;

std::vector<TCB*> threads;
std::vector<Mutex> mutexes;
std::vector<queue<TCB*>> readyQueue;

std::vector<MemoryPool*> poolsOfMemory;
TCB* currentlyRunningThread;

bool comparison(TCB* i, TCB* j)
{
	return (i->priority > j->priority);
}

void VMScheduler()
{
	MachineEnableSignals();
	TCB *previouslyRunningThread = currentlyRunningThread;

	if(previouslyRunningThread->state == VM_THREAD_STATE_RUNNING)
	{
		previouslyRunningThread->state = VM_THREAD_STATE_READY;
		readyQueue[previouslyRunningThread->state].push(previouslyRunningThread);
	}

	if(readyQueue[VM_THREAD_PRIORITY_HIGH].size() != 0)
	{
		currentlyRunningThread = readyQueue[VM_THREAD_PRIORITY_HIGH].front();
		readyQueue[VM_THREAD_PRIORITY_HIGH].pop();
	}

	else if(readyQueue[VM_THREAD_PRIORITY_NORMAL].size() != 0)
	{
		currentlyRunningThread = readyQueue[VM_THREAD_PRIORITY_NORMAL].front();
		readyQueue[VM_THREAD_PRIORITY_NORMAL].pop();
	}

	else if(readyQueue[VM_THREAD_PRIORITY_LOW].size() != 0)
	{
		currentlyRunningThread = readyQueue[VM_THREAD_PRIORITY_LOW].front();
		readyQueue[VM_THREAD_PRIORITY_LOW].pop();
	}

	else
	{
		currentlyRunningThread = readyQueue[0].front();
		readyQueue[0].pop();
	}
	
	currentlyRunningThread->state = VM_THREAD_STATE_RUNNING;
	MachineContextSwitch(&previouslyRunningThread->threadInfo, &currentlyRunningThread->threadInfo);
}

void idle(void*)
{
	MachineEnableSignals();
	while(true);
}

TVMStatus VMStart(int tickms, TVMMemorySize heapsize, TVMMemorySize sharedsize, int argc, char *argv[])
{		
	sharedData = MachineInitialize(sharedsize);//starting up the machine

	//System Memory
	MemoryPool* systemMemoryPool = new MemoryPool;
	MemoryBlock systemBlock;
	systemBlock.basePtr = new char[(int)heapsize];
	systemBlock.length = heapsize;
	systemBlock.freed = true;

	systemMemoryPool->blocks.push_back(systemBlock);
	systemMemoryPool->id = VM_MEMORY_POOL_ID_SYSTEM;
	systemMemoryPool->remainingSpace = heapsize;
	poolsOfMemory.push_back(systemMemoryPool);

	//Generate Memory Pools for All Other Process
	MemoryPool* sharedMemoryPool = new MemoryPool;
	MemoryBlock sharedMemoryBlock;
	sharedMemoryBlock.basePtr = new char[(int)sharedsize];
	sharedMemoryBlock.length = sharedsize;	
	sharedMemoryBlock.freed = true;
	sharedMemoryPool->id = 1;
	sharedMemoryPool->blocks.push_back(sharedMemoryBlock);
	sharedMemoryPool->remainingSpace = sharedsize;

	poolsOfMemory.push_back(sharedMemoryPool);

	MachineRequestAlarm(tickms*1000,VMAlarmCallBack, NULL);
	TVMMainEntry currentThread;
	currentThread = VMLoadModule(argv[0]);

	for(unsigned int i = 0; i < 4; i++)
	{
		std::queue<TCB*> priorities;
		readyQueue.push_back(priorities);
	}

	TCB* mainThread = new TCB;
	mainThread->tidNum = 0;
	mainThread->priority = VM_THREAD_PRIORITY_NORMAL;
	mainThread->state = VM_THREAD_STATE_RUNNING;
	threads.push_back(mainThread);
	currentlyRunningThread = mainThread;

	TVMThreadID threadID;
	VMThreadCreate(idle, NULL, 0x10000, (TVMThreadPriority)0x00, &threadID);
	VMThreadActivate(threadID);

	if(currentThread)//module successfully loaded
	{
		currentThread(argc,argv);//calling modules main function
		threads.clear();
		mutexes.clear();
		readyQueue.clear();
		poolsOfMemory.clear();
		return VM_STATUS_SUCCESS;
	}
	else
	{
		return VM_STATUS_FAILURE;
	}

}//end VMStart


TVMStatus VMFileWrite(int filedescriptor, void *data, int *length)
{
	if(data == NULL || length == NULL)
	{
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}
	globalFileFlag = 1;//set up to block	
	int numBytes = 512;
	memcpy(sharedData, data, numBytes);
	
	MachineFileWrite(filedescriptor, sharedData, *length, VMFileCallBack, NULL);
	while(globalFileFlag);
	
	if(fileDescriptor < 0)//failed to write file
	{
		return VM_STATUS_FAILURE;
	}
	else
	{;
		return VM_STATUS_SUCCESS;
	}

}//end VMFileWrite

TVMStatus VMFileOpen(const char *filename, int flags, int mode, int *filedescriptor)
{
	globalFileFlag = 1;//Have to wait until result comes through

	if(filename == NULL || filedescriptor == NULL)
	{
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}
	MachineFileOpen(filename, flags, mode, VMFileCallBack, NULL);
	while(globalFileFlag);//wait until get result

	*filedescriptor = fileDescriptor;
	if(fileDescriptor < 0)//failed to open file
	{
		return VM_STATUS_FAILURE;
	}
	else
	{
		return VM_STATUS_SUCCESS;
	}
}//end VMFileOpen

TVMStatus VMFileClose(int filedescriptor)
{
	globalFileFlag = 1;//will block until done

	MachineFileClose(filedescriptor, VMFileCallBack, NULL);
	while(globalFileFlag);
	
	if(fileDescriptor < 0)//failed to close file
	{
		return VM_STATUS_FAILURE;
	}
	else
	{
		return VM_STATUS_SUCCESS;
	}
	
}//end VMFileClose

TVMStatus VMFileRead(int filedescriptor, void *data, int *length)
{
	if(data == NULL || length == NULL)
	{
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}
	globalFileFlag = 1;//blocking

	MachineFileRead(filedescriptor, data, *length, VMFileCallBack, NULL);
	while(globalFileFlag);

	//*length = fileDescriptor;
	if(fileDescriptor < 0)//failed to read file
	{
		return VM_STATUS_FAILURE;
	}
	else
	{
		return VM_STATUS_SUCCESS;
	}
	
}//end VMFileRead

TVMStatus VMFileSeek(int filedescriptor, int offset, int whence, int *newoffset)
{
	globalFileFlag = 1;

	MachineFileSeek(filedescriptor, offset, whence, VMFileCallBack, NULL);
	while(globalFileFlag);

	if(newoffset)
	{
		*newoffset = fileDescriptor;
	}
	if(fileDescriptor < 0)//failed to read file
	{
		return VM_STATUS_FAILURE;
	}
	else
	{
		return VM_STATUS_SUCCESS;
	}
	
}//end VMFileSeek

void VMAlarmCallBack(void* calldata)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);
	tickCount++;

	for(auto it : threads)
	{
		if(it->sleepCount != 0)
		{
			it->sleepCount--;
		}
		else if(it->sleepCount == 0 && it->state == VM_THREAD_STATE_WAITING)
		{
			it->state = VM_THREAD_STATE_READY;
			readyQueue[it->priority].push(it);
			VMScheduler();
		}
	}
	
	MachineResumeSignals(&signalState);

}//end VMAlarmCallBack

void VMFileCallBack(void* calldata, int result)
{
	tickCount++;
	fileDescriptor = result;
	globalFileFlag = 0;//function can now resume
	
}//end VMFileCallBack

TVMStatus VMThreadCreate(TVMThreadEntry entry, void *param, TVMMemorySize memsize, TVMThreadPriority prio, TVMThreadIDRef tid)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);
	
	if(entry == NULL || tid == NULL)
	{
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}
	TCB *curTCB = static_cast<TCB*>(param);
	curTCB = new TCB;
	curTCB->memorysize = memsize;
	curTCB->priority = prio;
	curTCB->stack = new char[memsize];
	curTCB->state = VM_THREAD_STATE_DEAD;
	curTCB->entry = entry;
	curTCB->tidNum = threads.size()+1;
	*tid = curTCB->tidNum;

	skeleStruct *skeleton = new skeleStruct;
	skeleton->entry = entry;
	skeleton->param = param;
	
	SMachineContext newContext;
	MachineContextCreate(&newContext, VMThreadEntry, (void*)skeleton, curTCB->stack, curTCB->memorysize);
	curTCB->threadInfo = newContext;
	threads.push_back(curTCB);

	MachineResumeSignals(&signalState);
	return VM_STATUS_SUCCESS;
}//end VMThreadCreate

TVMStatus VMThreadID(TVMThreadIDRef threadref)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);
	for(unsigned int i = 0; i < threads.size(); i++)
	{
		if(threads[i]->state == VM_THREAD_STATE_RUNNING)
		{
			threadref = &threads[i]->tidNum;
			MachineResumeSignals(&signalState);
			return VM_STATUS_SUCCESS;
		}
	}
		MachineResumeSignals(&signalState);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
}
TVMStatus VMThreadDelete(TVMThreadID thread)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	for(unsigned int i = 0; i < threads.size(); i++)
	{
		int counter = 0;
		if(threads[i]->tidNum == thread && threads[i]->state == VM_THREAD_STATE_DEAD)
		{
			threads.erase(threads.begin()+counter);
			MachineResumeSignals(&signalState);
			return VM_STATUS_SUCCESS;
		}
		else if(threads[i]->tidNum == thread && (threads[i]->state == VM_THREAD_STATE_RUNNING 
			|| threads[i]->state == VM_THREAD_STATE_READY ||  threads[i]->state == VM_THREAD_STATE_WAITING))
		{
			MachineResumeSignals(&signalState);
			return VM_STATUS_ERROR_INVALID_STATE;
		}
	}
	
	MachineResumeSignals(&signalState);	
	return VM_STATUS_ERROR_INVALID_ID;

}

TVMStatus VMThreadState(TVMThreadID thread, TVMThreadStateRef stateref)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	if(stateref == NULL)
	{
		MachineResumeSignals(&signalState);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	for(unsigned int i = 0; i < threads.size(); i++)
	{
		if(threads[i]->tidNum == thread)
		{
			*stateref = threads[i]->state;
			MachineResumeSignals(&signalState);
			return VM_STATUS_SUCCESS;
		}
	}
	MachineResumeSignals(&signalState);
	return VM_STATUS_ERROR_INVALID_ID;
}

//Do Mutex for this one
TVMStatus VMThreadTerminate(TVMThreadID thread)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	for(unsigned int i = 0; i < threads.size(); i++)
	{
		if(threads[i]->tidNum == thread)
		{
			threads[i]->state = VM_THREAD_STATE_DEAD;
			
			for(auto it : mutexes)
			{
				if(it.threadID == thread)
				{
					VMMutexRelease(it.mutexID);
				}
			}
			VMScheduler();
			MachineResumeSignals(&signalState);
			return VM_STATUS_SUCCESS;
		}
		else if(threads[i]->tidNum == thread && threads[i]->state == VM_THREAD_STATE_DEAD)
		{
			MachineResumeSignals(&signalState);
			return VM_STATUS_ERROR_INVALID_STATE;
		}
	}
	
	MachineResumeSignals(&signalState);
	return VM_STATUS_ERROR_INVALID_ID;
}

TVMStatus VMThreadActivate(TVMThreadID thread)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	for(unsigned int i = 0; i < threads.size(); i++)
	{
		if(threads[i]->tidNum == thread && threads[i]->state == VM_THREAD_STATE_DEAD)
		{
			threads[i]->state = VM_THREAD_STATE_READY;
			readyQueue[threads[i]->priority].push(threads[i]);
			MachineResumeSignals(&signalState);
			return VM_STATUS_SUCCESS;
		}
		else if(threads[i]->tidNum == thread && threads[i]->state != VM_THREAD_STATE_DEAD)
		{
			MachineResumeSignals(&signalState);
			return VM_STATUS_ERROR_INVALID_STATE;
		}
	}

	MachineResumeSignals(&signalState);	
	return VM_STATUS_ERROR_INVALID_ID;
}

TVMStatus VMTickCount(TVMTickRef tickref)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	TVMTick tickCount;
	tickref = &tickCount;

	if(tickref == NULL)
	{
		MachineResumeSignals(&signalState);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	MachineResumeSignals(&signalState);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMTickMS(int *tickmsref)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);	
	
	int ticks = tickCount * 1000;
	tickmsref = &ticks;

	if(tickmsref == NULL)
	{
		MachineResumeSignals(&signalState);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	MachineResumeSignals(&signalState);
	return VM_STATUS_SUCCESS;
}


void VMThreadEntry(void *skeleton)
{
	TVMThreadEntry function = ((skeleStruct*)skeleton)->entry;
	function(((skeleStruct*)skeleton)->param);
	delete((skeleStruct*)skeleton);	
	VMThreadTerminate(currentlyRunningThread->tidNum);
}//end VMThreadEntry

TVMStatus VMThreadSleep(TVMTick tick)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	if(tick == VM_TIMEOUT_IMMEDIATE)
	{
		currentlyRunningThread->state = VM_THREAD_STATE_READY;
		readyQueue[currentlyRunningThread->priority].push(currentlyRunningThread);
		VMScheduler();
		MachineResumeSignals(&signalState);
	}
	else if(tick == VM_TIMEOUT_INFINITE)
	{
		MachineResumeSignals(&signalState);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}
	else
	{
		currentlyRunningThread->sleepCount = tick;
		currentlyRunningThread->state = VM_THREAD_STATE_WAITING;
		VMScheduler();
		MachineResumeSignals(&signalState);
	}
	return VM_STATUS_SUCCESS;
}//end VMThreadSleep

TVMStatus VMMutexAcquire(TVMMutexID mutex, TVMTick timeout)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);	

	for(unsigned int i = 0; i < mutexes.size(); i++)
	{
		if(mutex == mutexes[i].mutexID)
		{
			if(mutexes[i].isLocked == true)
			{
				if(timeout == VM_TIMEOUT_IMMEDIATE)
				{
					MachineResumeSignals(&signalState);
					return VM_STATUS_FAILURE;
				}
				
				else if(timeout == VM_TIMEOUT_INFINITE)
				{
					while(true)
					{
						if(mutexes[i].isLocked == false)
						{
							MachineResumeSignals(&signalState);
							return VM_STATUS_SUCCESS;
						}
					}
					MachineResumeSignals(&signalState);
					return VM_STATUS_FAILURE;
				}
				else
				{
					while(timeout > 0)
					{
						if(mutexes[i].isLocked == false)
						{
							MachineResumeSignals(&signalState);
							return VM_STATUS_SUCCESS;
						}
					}
					MachineResumeSignals(&signalState);
					return VM_STATUS_FAILURE;
				}

			}
		}
	}
	MachineResumeSignals(&signalState);
	return VM_STATUS_ERROR_INVALID_ID;
}

TVMStatus VMMutexRelease(TVMMutexID mutex)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	for(unsigned int i = 0; i < mutexes.size(); i++)
	{
		if(mutex == mutexes[i].mutexID)
		{
			if(mutexes[i].isLocked == true)
			{
				mutexes[i].isLocked = false;
				MachineResumeSignals(&signalState);
				return VM_STATUS_SUCCESS;
			}
			else
			{
				MachineResumeSignals(&signalState);
				return VM_STATUS_ERROR_INVALID_STATE;
			}
		}
	}
	MachineResumeSignals(&signalState);
	return VM_STATUS_ERROR_INVALID_ID;
}

TVMStatus VMMutexQuery(TVMMutexID mutex, TVMThreadIDRef ownerref)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	if(ownerref == NULL)
	{
		MachineResumeSignals(&signalState);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	for(unsigned int i = 0; i < mutexes.size(); i++)
	{
		if(mutex == mutexes[i].mutexID)
		{
			*ownerref = mutexes[i].threadID;
			return VM_STATUS_SUCCESS;
		}
	}

	MachineResumeSignals(&signalState);
	return VM_STATUS_ERROR_INVALID_ID;
}

TVMStatus VMMutexDelete(TVMMutexID mutex)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	for(unsigned int i = 0; i < mutexes.size(); i++)
	{
		if(mutex == mutexes[i].mutexID)
		{
			if(mutexes[i].isLocked == false)
			{
				if(mutexes[i].threadID <= 0)
				{
					mutexes.erase(mutexes.begin()+i);
					MachineResumeSignals(&signalState);
					return VM_STATUS_SUCCESS;
				}
				else
				{
					MachineResumeSignals(&signalState);
					return VM_STATUS_ERROR_INVALID_STATE;
				}
			}
		}
	}
	
	MachineResumeSignals(&signalState);
	return VM_STATUS_ERROR_INVALID_ID;
}

TVMStatus VMMutexCreate(TVMMutexIDRef mutexref)
{
	TMachineSignalState signalState;
	MachineSuspendSignals(&signalState);

	if(mutexref == NULL)
	{
		MachineResumeSignals(&signalState);
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	Mutex newMutex;
	newMutex.isLocked = false;
	newMutex.threadID = -1;
	
	if(mutexes.size() > 0)
	{
		newMutex.mutexID = mutexes.size();
	}
	else
	{
		newMutex.mutexID = 0;
	}

	mutexes.push_back(newMutex);
	*mutexref = newMutex.mutexID;

	MachineResumeSignals(&signalState);
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMemoryPoolCreate(void *base, TVMMemorySize size, TVMMemoryPoolIDRef memory)
{
	if(base == nullptr || memory == nullptr || size == 0)
	{
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	MemoryPool* newMemory = new MemoryPool;
	newMemory->id = poolsOfMemory.size();

	MemoryBlock newBlock;
	newBlock.length = size;
	newBlock.freed = true;
	newBlock.basePtr = (char*)base;
	newMemory->blocks.push_back(newBlock);
	newMemory->remainingSpace = size;	
	poolsOfMemory.push_back(newMemory);
	*memory = newMemory->id;
	return VM_STATUS_SUCCESS;
}

TVMStatus VMMemoryPoolDelete(TVMMemoryPoolID memory)
{
	std::vector<MemoryPool*>::iterator counter = poolsOfMemory.begin();
	for(auto it : poolsOfMemory)
	{
		if(it->id == memory)
		{
			for(auto it2 : it->blocks)
			{
				if(it2.freed == false)
				{
					return VM_STATUS_ERROR_INVALID_STATE;
				}
			}
			poolsOfMemory.erase(counter);
			return VM_STATUS_SUCCESS;
		}
		counter++;
	}

	return VM_STATUS_ERROR_INVALID_STATE;
}


TVMStatus VMMemoryPoolAllocate(TVMMemoryPoolID memory, TVMMemorySize size, void **pointer)
{
	if(size == 0 || pointer == nullptr)
	{
		return VM_STATUS_ERROR_INVALID_PARAMETER;
	}

	for(auto it : poolsOfMemory)
	{
		if(it->id == memory)
		{
			if((size % 64) != 0)
			{
				size = (size + 0x3F)&(~0x3F);
			}
			
			if(it->remainingSpace >= size)
			{
				int counter = 0;
				for(auto &itera2 : it->blocks)
				{
					if(itera2.length >= size && itera2.freed == true)
					{
						*pointer = itera2.basePtr;
						itera2.freed = false;
						it->remainingSpace -= size;

						if(it->remainingSpace > 0)
						{
							MemoryBlock newBlock;
							newBlock.length = it->remainingSpace;
							newBlock.basePtr = itera2.basePtr + size;
							newBlock.freed = true;
							it->blocks.push_back(newBlock);		
						}
						return VM_STATUS_SUCCESS;
					}
					counter++;
				}
			}
			else
			{
				return VM_STATUS_ERROR_INSUFFICIENT_RESOURCES;
			}
		}
	}
	return VM_STATUS_ERROR_INVALID_PARAMETER;
}

TVMStatus VMMemoryPoolDeallocate(TVMMemoryPoolID memory, void *pointer)
{
	for(auto it : poolsOfMemory)
	{
		if(it->id == memory)
		{
			for(auto &it2 : it->blocks)
			{
				if(it2.basePtr == (char*)pointer && it2.freed == false)
				{
					it2.freed = true;
					it->remainingSpace += it2.length;
					return VM_STATUS_SUCCESS;
				}
			}
			return VM_STATUS_ERROR_INVALID_PARAMETER;
		}
	}

	return VM_STATUS_ERROR_INVALID_PARAMETER;
}


TVMStatus VMMemoryPoolQuery(TVMMemoryPoolID memory, TVMMemorySizeRef bytesleft)
{

	for(auto it : poolsOfMemory)
	{
		if(it->id == memory)
		{
			*bytesleft = it->remainingSpace;
			if(bytesleft == NULL)
			{
				return VM_STATUS_ERROR_INVALID_PARAMETER;
			}
			return VM_STATUS_SUCCESS;
		}
	}

	return VM_STATUS_ERROR_INVALID_PARAMETER;
}
