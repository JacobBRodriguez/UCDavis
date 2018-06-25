#include <iostream>
#include <fstream>
#include <iomanip>
#include "BinaryTree.h"
#include "BinaryHeap.h"

using namespace std;
BinaryTree::BinaryTree() {

}

void BinaryTree::setLeftSibling(BinaryTree* left) {
	this->leftSibling = left;
	left->setParent(this);
}

void BinaryTree::setRightSibling(BinaryTree* right) {
	this->rightSibling = right;
	right->setParent(this);
}

void BinaryTree::setParent(BinaryTree* p) {
	this->parent = p;
}

void BinaryTree::setWeight() {
	this->weight = ((leftSibling->weight)+(rightSibling->weight));
}//end setWeight

void BinaryTree::printTree(BinaryTree* root, int depth, char* encoding, int lOrR) {
	int temp = 0, temp2 = 1;
	if(lOrR == 0) {
		encoding[depth-1] = '0';
		encoding[depth] = '\0';	
	}
	if(lOrR == 1) {
		encoding[depth-1] = '1';
		encoding[depth] = '\0';	
	}
	depth++;

	if(root->leftSibling == NULL && root->rightSibling == NULL)
		{
		cout << root->letter<<"  "<<setw(3)<<root->weight<< " "<<setw(3)<< encoding<<endl;
			return;
		}//end if
	
	printTree(root->leftSibling, depth,encoding, temp);
	printTree(root->rightSibling, depth,encoding,temp2);		
		
}//end printTree



int main(int argc, char *argv[]) {
 	
	int numOfLetter[256];
	for(int i = 0; i < 256; i++)
		numOfLetter[i] = 0;

	char tempLetter;
	int temp;
	
	ifstream inf(argv[1]);

	
	while(inf.get(tempLetter)) {
		
		temp = (int)tempLetter;
		numOfLetter[temp]++;
	}//end while
	
	BinaryHeap<BinaryTree> heap;
	BinaryTree *ptr3 = new BinaryTree();
	char encoding[500];
	for(int i = 0; i < 500; i++)
		encoding[i] = '0';

	
	for(int i = 0; i < 256; i++) {
		if(numOfLetter[i] > 0) {
			tempLetter = (char)i;
			BinaryTree tempNode;
			tempNode.weight = numOfLetter[i];
			tempNode.letter = tempLetter;
			tempNode.base = true;
			tempNode.leftSibling = NULL;
			tempNode.rightSibling = NULL;
			heap.insert(tempNode);
		}//end if
	}//end for
	while(true) {
		BinaryTree tempParent;
		tempParent.base = false;

		BinaryTree *ptr1 = new BinaryTree(); 
		heap.deleteMin(*ptr1);

		BinaryTree *ptr2 = new BinaryTree();
		heap.deleteMin(*ptr2);

		tempParent.leftSibling = ptr1;
		tempParent.rightSibling = ptr2;
		tempParent.weight = (ptr1->weight + ptr2->weight);

		if(heap.isEmpty())//parent has taken all children
			{
				int tom = 0, first = 3;
				ptr3 = &tempParent;
				tempParent.printTree(ptr3,tom,encoding,first);
				break;
			}//end if	
		heap.insert(tempParent);
	}//end while
	
}//end main

