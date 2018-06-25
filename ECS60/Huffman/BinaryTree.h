#ifndef BinaryTreeH
#define BinaryTreeH

#include <iostream>
#include <fstream>
#include <iomanip>
#include "BinaryHeap.h"
class BinaryTree
{
public:
  BinaryTree* leftSibling;
  BinaryTree* rightSibling;
  BinaryTree* parent;
  int weight;
  bool base;
  char letter;
  
  
public:
  BinaryTree();
  BinaryTree insert(BinaryTree);
  void print();
  void setLeftSibling(BinaryTree* left);
  void setRightSibling(BinaryTree* right);
  void setParent(BinaryTree* p);
  void setWeight();
  void printTree(BinaryTree* root, int depth, char* encoding, int lOrR);
  bool operator<(const BinaryTree& rhs) const {
		return this->weight < rhs.weight;
	}
  
  

}; // BinaryTree class
#endif

