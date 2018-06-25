#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  BTreeNode* addPtr(BTreeNode *ptr, int pos);
  void addToLeft(BTreeNode *last);
  void borrowToLeft();//added function
  void afterBorrowToLeft();//added function
  void mergeToLeft();//added function
  void addToRight(BTreeNode *ptr, BTreeNode *last);
  void borrowToRight();//added function
  void afterBorrowToRight();//added function
  void mergeToRight();//added function
  void dealWithMerge(int i);//added function
  void addToThis(BTreeNode *ptr, int pos); // pos is where the node should go
  void deleteToThis(int value);//added function
  int getMaximum() const;
  BTreeNode* getIMaximum();//added function
  BTreeNode* getIMinimum();//added function
  int getMinimum() const;
  InternalNode* insert(int value); // returns pointer to new
    // InternalNode if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  BTreeNode* remove(int value);
  void resetMinimum(const BTreeNode* child);
  InternalNode* split(BTreeNode *last);
}; // InternalNode class



#endif
