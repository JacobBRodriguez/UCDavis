#include <iostream>
#include <climits>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


BTreeNode* InternalNode::addPtr(BTreeNode *ptr, int pos)
{ // called when original was full, pos is where the node belongs.
  if(pos == internalSize)
    return ptr;

  BTreeNode *last = children[count - 1];

  for(int i = count - 2; i >= pos; i--)
  {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
  } // shift things to right to make room for ptr, i can be -1!

  children[pos] = ptr;  // i will end up being the position that it is inserted
  keys[pos] = ptr->getMinimum();
  ptr->setParent(this);
  return last;
} // InternalNode:: addPtr()


void InternalNode::addToLeft(BTreeNode *last)
{
  ((InternalNode*)leftSibling)->insert(children[0]);

  for(int i = 0; i < count - 1; i++)
  {
    children[i] = children[i + 1];
    keys[i] = keys[i + 1];
  }

  children[count - 1] = last;
  keys[count - 1] = last->getMinimum();
  last->setParent(this);
  if(parent)
    parent->resetMinimum(this);
} // InternalNode::ToLeft()


void InternalNode::addToRight(BTreeNode *ptr, BTreeNode *last)
{
  ((InternalNode*) rightSibling)->insert(last);
  if(ptr == children[0] && parent)
    parent->resetMinimum(this);
} // InternalNode::addToRight()



void InternalNode::addToThis(BTreeNode *ptr, int pos)
{  // pos is where the ptr should go, guaranteed count < internalSize at start
  int i;

  for(i = count - 1; i >= pos; i--)
  {
      children[i + 1] = children[i];
      keys[i + 1] = keys[i];
  } // shift to the right to make room at pos

  children[pos] = ptr;
  keys[pos] = ptr->getMinimum();
  count++;
  ptr->setParent(this);

  if(pos == 0 && parent)
    parent->resetMinimum(this);
} // InternalNode::addToThis()



int InternalNode::getMaximum() const
{
  if(count > 0) // should always be the case
    return children[count - 1]->getMaximum();
  else
    return INT_MAX;
}  // getMaximum();


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{  // count must always be >= 2 for an internal node
  int pos; // will be where value belongs

  for(pos = count - 1; pos > 0 && keys[pos] > value; pos--);

  BTreeNode *last, *ptr = children[pos]->insert(value);
  if(!ptr)  // child had room.
    return NULL;

  if(count < internalSize)
  {
    addToThis(ptr, pos + 1);
    return NULL;
  } // if room for value

  last = addPtr(ptr, pos + 1);

  if(leftSibling && leftSibling->getCount() < internalSize)
  {
    addToLeft(last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < internalSize)
    {
      addToRight(ptr, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(last);
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count = 2;
  children[0]->setLeftSibling(NULL);
  children[0]->setRightSibling(children[1]);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  oldRoot->setParent(this);
  node2->setParent(this);
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *newNode)
{ // called by sibling so either at beginning or end
  int pos;

  if(newNode->getMinimum() <= keys[0]) // from left sibling
    pos = 0;
  else // from right sibling
    pos = count;

  addToThis(newNode, pos);
} // InternalNode::insert(BTreeNode *newNode)


void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()





//****************************************************************
BTreeNode* InternalNode::remove(int value)
{ 
  	deleteToThis(value);

	if(internalSize%2 != 0)
		internalSize++;
	
	if(this->getCount() < (internalSize/2)) //The leaf is below minimum
	{
		if(((InternalNode*)leftSibling) && ((InternalNode*)leftSibling)->getCount() > (internalSize/2))//left has more than min
		{
			borrowToLeft();
			return NULL;
		}//end if 2
		else if(((InternalNode*)leftSibling) && (this->getCount()+(((InternalNode*)leftSibling)->getCount())) <= internalSize)
			{
				mergeToLeft();
				return NULL;
			}

		else { // left sibling at min or non-existent
    if(((InternalNode*)rightSibling) && ((InternalNode*)rightSibling)->getCount() > (internalSize/2))
    			{
      				borrowToRight();
      				return NULL;
    			}//end if 3 
			else if(((InternalNode*)rightSibling) && (this->getCount()+(((InternalNode*)rightSibling)->getCount())) <= internalSize)
				{
					mergeToRight();
					return NULL;
				}
      		}//end else 1 
	}//end if 1 

if((count == 1) && (((InternalNode*)leftSibling) == NULL) && (((InternalNode*)rightSibling) == NULL)) 
		{
			children[0]->setParent(NULL);
			return children[0];
		}//end if

return NULL;
} // InternalNode::remove(int value)



void InternalNode::deleteToThis(int value)
{  // pos is where the ptr should go, guaranteed count < internalSize at start
  int i, temp = 0;
  BTreeNode *ptr;

  for(i = 0; i < count - 1; i++)//finding correct place of value
  {
      if((keys[i] <= value) && (value <= children[i]->getMaximum()))
		break;
	temp++;
  }

  ptr = children[temp]->remove(value);
  if(ptr) {
	delete ptr;
	for(;temp <= count - 2; temp++){//shift everything over to fill gap
		keys[temp] = keys[temp+1];
		children[temp] = children[temp+1];
	}//end for
	count--;
 }
	for(int i = 0; i < count; i++)
  		resetMinimum(children[i]); //resetting keys if needed
	
} // InternalNode::deleteToThis()

void InternalNode::borrowToLeft() {
	BTreeNode *ptr;
	ptr = ((InternalNode*)leftSibling)->getIMaximum();
	insert(ptr);
	((InternalNode*)leftSibling)->afterBorrowToLeft();
	
} // LeafNode::borrowToLeft()

void InternalNode::borrowToRight() {
	BTreeNode *ptr;
	ptr = ((InternalNode*)rightSibling)->getIMinimum();
	insert(ptr);
	((InternalNode*)rightSibling)->afterBorrowToRight();
	
} // LeafNode::borrowToRight()

void InternalNode::mergeToLeft() {
	int temp = 0;
	while(count > 0) {
		((InternalNode*)leftSibling)->insert(this->children[temp]);
		count--;
		temp++;
	}//end while
	((InternalNode*)leftSibling)->setRightSibling(NULL);
	parent->dealWithMerge(1);
}//end LeafNode::mergeToLeft()

void InternalNode::mergeToRight() {
	while(count > 0) {
		((InternalNode*)rightSibling)->insert(this->children[count-1]);
		count--;
	}//end while
	((InternalNode*)rightSibling)->setLeftSibling(NULL);
	parent->dealWithMerge(0);
}//end LeafNode::mergeToRight()


BTreeNode* InternalNode::getIMaximum() {
	if(count > 0)
		return children[count-1];
	else
		return NULL;
return NULL;

}//end InternalNode::getMaximum()

BTreeNode* InternalNode::getIMinimum() {
	if(count > 0)
		return children[0];
	else
		return NULL;
return NULL;

}//end InternalNode::getMinimum()


void InternalNode::afterBorrowToLeft() {
	count--;

}//end InternalNode::afterBorrowToLeft(*BTreeNode ptr)

void InternalNode::afterBorrowToRight() {
	int temp = 0;
  	for(;temp <= count - 2; temp++){//shift everything over
		children[temp] = children[temp+1];
		keys[temp] = keys[temp+1];
	}
	count--;

    	parent->resetMinimum(children[0]);

}//end InternalNode::afterBorrowToRight(*BTreeNode ptr)


void InternalNode::dealWithMerge(int i) {
	int temp = 0;
	if(i == 1)
		count--;
	else {
	for(;temp <= count - 2; temp++){//shift everything over to fill gap
		keys[temp] = keys[temp+1];
		children[temp] = children[temp+1];
	}//end for
	count--;
	}
	resetMinimum(children[0]);

	
}//end InternalNode::dealWithMerge(BTreeNode* ptr)

//****************************************************************

void InternalNode::resetMinimum(const BTreeNode* child)
{
  for(int i = 0; i < count; i++)
    if(children[i] == child)
    {
      keys[i] = children[i]->getMinimum();
      if(i == 0 && parent)
        parent->resetMinimum(this);
      break;
    }
} // InternalNode::resetMinimum()


InternalNode* InternalNode::split(BTreeNode *last)
{
  InternalNode *newptr = new InternalNode(internalSize, leafSize,
    parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(newptr);

  rightSibling = newptr;

  for(int i = (internalSize + 1) / 2; i < internalSize; i++)
  {
    newptr->children[newptr->count] = children[i];
    newptr->keys[newptr->count++] = keys[i];
    children[i]->setParent(newptr);
  }

  newptr->children[newptr->count] = last;
  newptr->keys[newptr->count++] = last->getMinimum();
  last->setParent(newptr);
  count = (internalSize + 1) / 2;
  return newptr;
} // split()
