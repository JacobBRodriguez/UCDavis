// Author Me
#ifndef defragmenterH
  #define defragmenterH

#include "DefragRunner.h"
#include "QuadraticProbingPtr.h"



class Defragmenter
{
public:
  DiskDrive *diskDrive;
  DiskBlock* block;
  Defragmenter();
  void moveBlocks(DiskDrive *diskDrive);
  int checkBlock(int blockNum, QuadraticPtrHashTable<SavingPrivateRyan> aTable);
  Defragmenter(DiskDrive *diskDrive){
	moveBlocks(diskDrive);
}

}; // class Defragmenter

#endif
