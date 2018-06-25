#include "mynew.h"
#include "DefragRunner.h"
#include "defragmenter.h"
using namespace std;

void Defragmenter::moveBlocks(DiskDrive *diskDrive) {
	
	int tempBlockNum, firstID, trackOfFat, i = 2, ID = 0,check, howFar = 2, sizeOfDisk,size;
	SavingPrivateRyan miller;

	QuadraticPtrHashTable<SavingPrivateRyan> table(miller, 50007);

	sizeOfDisk = diskDrive->getCapacity();

		trackOfFat = sizeOfDisk-1;
		size = diskDrive->directory->getSize();
	
		DiskBlock* block = new DiskBlock;
		firstID = diskDrive->directory[ID].getFirstBlockID();//first block of first file
		block = diskDrive->readDiskBlock(firstID);//setting block to first block
		
while(i <= size){
		
	while(check != 1) {//not end of file. Special case.
		
		if(diskDrive->FAT[i] == true) { //there is something in that spot
			while(trackOfFat != 2) {//search for empty spot

				if(diskDrive->FAT[trackOfFat] == false) {//that space is empty

					DiskBlock* tempBlock = new DiskBlock;
					tempBlock = diskDrive->readDiskBlock(i);
					tempBlockNum = trackOfFat;

					SavingPrivateRyan james;
					james.thisCurrentBlock = i;
					james.blockRyanWentTo = tempBlockNum;
					table.insert(james);//inserting class into hash
					diskDrive->writeDiskBlock(tempBlock,tempBlockNum);
					delete(tempBlock);//deallocating memory

					diskDrive->FAT[trackOfFat] = true;//slot now taken
					diskDrive->FAT[i] = false; 
					break;
				}//end if1.2
				trackOfFat--;
				if(trackOfFat == 2)
					trackOfFat = sizeOfDisk-1;//back to the end to search
			}//end while
		}//end if1
			int saveTu = block->getNext();//getting next block if works
			
			
			
			diskDrive->writeDiskBlock(block, i);//writing block to disk
			delete(block);//delete after write?
			block->setNext(saveTu);//do I need to set values?


			diskDrive->FAT[firstID] = false;
			diskDrive->FAT[i] = true;
			
			
			firstID = saveTu;
			check = firstID;
			
			if(check == 1)
				break;
			
			
			
			firstID = checkBlock(firstID, table);
			i++;
			DiskBlock* block = new DiskBlock;//new block everytime?
			
			block = diskDrive->readDiskBlock(firstID);//not reading in correct block
			
			
		
			
		}//end while
		
		diskDrive->directory[ID].setFirstBlockID(howFar);

		delete(block);
		DiskBlock* block = new DiskBlock;
		ID++;
		firstID = diskDrive->directory[ID].getFirstBlockID();
		block = diskDrive->readDiskBlock(firstID);
		i++;
		check = 0;
		if(i >= size)
			break;
		howFar = i;
	}//end while
}//end defragmenter::moveBlocks()


int Defragmenter::checkBlock(int blockNum,QuadraticPtrHashTable<SavingPrivateRyan> aTable ) {

	SavingPrivateRyan tempFrancis;
	tempFrancis.thisCurrentBlock = blockNum;
	tempFrancis = aTable.find(tempFrancis);
	int goTo = tempFrancis.blockRyanWentTo;

	if(goTo != -1) {//the block has been altered
		return checkBlock(goTo, aTable);

	}//end if1
	return blockNum;
}//end Defragmenter::checkBlock(int blockNum)






