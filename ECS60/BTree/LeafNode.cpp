#include <iostream>
#include <climits>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

void LeafNode::addToLeft(int value, int last)
{
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::ToLeft()

void LeafNode::addToRight(int value, int last)
{
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

void LeafNode::addToThis(int value)
{
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis()


void LeafNode::addValue(int value, int &last)
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode:: addValue()


int LeafNode::getMaximum()const
{
  if(count > 0)  // should always be the case
    return values[count - 1];
  else
    return INT_MAX;
} // getMaximum()


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)
  {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)
  {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


//*************************************************************************************


LeafNode* LeafNode::remove(int value)
{  
	deleteToThis(value);
	if(leafSize%2 != 0)
		leafSize++;
	
	if(this->getCount() < (leafSize/2)) //The leaf is below minimum
	{
		if(leftSibling && leftSibling->getCount() > (leafSize/2))//left has more than min
		{
			borrowToLeft();
			return NULL;
		}//end if 2
		else if(leftSibling && (this->getCount()+leftSibling->getCount()) <= leafSize)
				return (this->mergeToLeft());

		else { // left sibling at min or non-existent
    			if(rightSibling && rightSibling->getCount() > (leafSize/2))
    			{
      				borrowToRight();
      				return NULL;
    			}//end if 3
			else if(rightSibling && (this->getCount()+rightSibling->getCount()) <= leafSize)
					return (this->mergeToRight());
      		}//end else 1
	}//end if 1
	else
		return NULL;
return NULL;
}// LeafNode::remove()


void LeafNode::deleteToThis(int value)
{
  int i, temp = 0;

  for(i = 0; i < count; i++) {//find where the value is in array
	if(values[i] == value)
		break;
	if(values[i] == values[count])//value is not in the array
		return;
	temp++;
	}
  for(;temp <= count - 2; temp++){//shift everything over
	values[temp] = values[temp+1];
	}

  	count--;
} // LeafNode::deleteToThis()

void LeafNode::borrowToLeft() {
	int numberBorrow;
	numberBorrow = leftSibling->getMaximum();
	leftSibling->remove(numberBorrow);
	insert(numberBorrow);
} // LeafNode::borrowToLeft()

void LeafNode::borrowToRight() {
	int numberBorrow;
	numberBorrow = rightSibling->getMinimum();
	rightSibling->remove(numberBorrow);
	insert(numberBorrow);

} // LeafNode::borrowToRight()

LeafNode* LeafNode::mergeToLeft() {
	int temp = 0;
	while(count > 0) {
		leftSibling->insert(values[temp]);
		count--;
		temp++;
	}//end while
	leftSibling->setRightSibling(NULL);
	return this;

}//end LeafNode::mergeToLeft()

LeafNode* LeafNode::mergeToRight() {
	while(count > 0) {
		rightSibling->insert(values[count-1]);
		count--;
	}//end while
	rightSibling->setLeftSibling(NULL);
	return this;

}//end LeafNode::mergeToRight()



//**************************************************************************************

LeafNode* LeafNode::split(int value, int last)
{
  LeafNode *ptr = new LeafNode(leafSize, parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(ptr);

  rightSibling = ptr;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
    ptr->values[ptr->count++] = values[i];

  ptr->values[ptr->count++] = last;
  count = (leafSize + 1) / 2;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
  return ptr;
} // LeafNode::split()
