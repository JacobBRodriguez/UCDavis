#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  void addToLeft(int value, int last);
  void borrowToLeft();//added function
  LeafNode* mergeToLeft();//added function
  void addToRight(int value, int last);
  void borrowToRight();//added function
  LeafNode* mergeToRight();//added function
  void addToThis(int value);
  void deleteToThis(int value);//added function
  void addValue(int value, int &last);
  int getMaximum() const;
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  LeafNode* remove(int value); // NULL == no merge
  void print(Queue <BTreeNode*> &queue);
  LeafNode* split(int value, int last);

}; //LeafNode class

#endif
