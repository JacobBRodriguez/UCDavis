// JAcob Rodriguez

#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <sstream>
#include <vector>
#include <cctype>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>



// Code Found on stackOverflow to check if given string contains letters of the alphabet by comparing each character in string to given isalpha function
// Returns true if string contains alphabetical items
bool checkAlpha(const std::string &str)
{
    for(int i = 0; i < str.size(); i++)
        if( isalpha(str[i]) || isspace(str[i]))
            return true;
    return false;
} // end checkAlpha **************************************************************************************************************************

int isOperator(std::string incoming)
{
	//std::string plus = "+";

	if(incoming.compare("+") == 0)
		return 1;

	else if(incoming.compare("-") == 0)
		return 2;

	else if(incoming.compare("*") == 0)
		return 3;

	else if(incoming.compare("/") == 0)
		return 4;

	else if(incoming.compare("^") == 0)
		return 5;

	else if(incoming.compare("<") == 0)
		return 6;
	
	else if(incoming.compare(">") == 0)
		return 7;
	
	else if(incoming.empty())
		return 9;

	else if(checkAlpha(incoming) == true)
		return 8;
	
	else
	{
		try
		{
			double stuff;
			stuff = atof(incoming.c_str()); // try to convert to double
			return 0;
		}
		
		catch(...)
		{
			std::cout << "Error: Invalid input '"<<incoming<<"'";
		}
	}
		

return 8;
	
}//end isOperator  ***************************************************************************************


void dashLOption(std::vector<std::string> holding)
{
	std::string whiteSpace = "    ", character, prevChar;
	int opCount = 0, spaceCount = 0, parenCount = 0;
	while(holding.empty() == 0)//while we still have items on stack
	{
		character = holding.back();//get top item on stack
		holding.pop_back();//remove top item on stack
		if(isOperator(character) == 1 || isOperator(character) == 2 || isOperator(character) == 3 || isOperator(character) == 4 || isOperator(character) == 5 ) //one of main operators
		{
			for(int i = 0; i < spaceCount; i++)//give a whiteSpace per indention ammount before putting operator on level
			{
				std::cout<<whiteSpace;
			}//end for

			std::cout<<"("<<character<<std::endl;//output operator
			parenCount++; // increase count
			spaceCount += 1;//increase indention count for next item
			opCount += 1;//increase count to know how far back need to get
			prevChar = "+";//Know that numbers to unroll is 2
		}// end if main operator

		else if(isOperator(character) == 6 || isOperator(character) == 7) //two operators that only require single number output
		{
			for(int i = 0; i < spaceCount; i++)//give a whiteSpace per indention ammount before putting operator on level
			{
				std::cout<<whiteSpace;
			}//end for

			std::cout<<"("<<character<<std::endl;//output operator
			parenCount++;
			spaceCount += 1;//increase indention count for next item
			opCount += 1;//increase count to know how far back need to get
			prevChar = "<";//Know that numbers to unroll is only 1
		}//end else if 6 || 7

		else if(isOperator(character) == 8) // number is alphabetical, so give error. Most likely not used code
		{
			std::cout << "Error: Invalid input '"<<character<<"'";
		}// end else if 8

		else if(isOperator(character) == 0) // number is actual number so have to put and backtrack
		{
			for(int i = 0; i < spaceCount; i++)//give a whiteSpace per indention amount before putting operator on level
			{
				std::cout<<whiteSpace;
			}//end for
			
			if(prevChar.compare("<") == 0) // only one operator comes off the stack
			{
				std::cout<<character<<std::endl;
				spaceCount -= 1; //decrease spaceCount
				opCount -= 1; //decrease opCount

				for(int i = 0; i < spaceCount; i++)//give a whiteSpace per indention amount before putting operator on level
				{
					std::cout<<whiteSpace;
				}//end for
				std::cout<<")"<<std::endl;
				parenCount--;
				prevChar = ")"; //assignment for number that follows lower level

			}// end if
			
			else if(prevChar.compare(")") == 0) //only one comes off and back more
			{
				std::cout<<character<<std::endl;
				spaceCount -= 1; //decrease spaceCount
				opCount -= 1; //decrease opCount
			}// end else if


			else // two operators come off the stack
			{
				std::cout<<character<<std::endl;

				if(holding.empty() == 0) //items still remain on the stack
				{
					character = holding.back();//get top item on stack
					holding.pop_back();//remove top item on stack

					if(isOperator(character) == 0) // item is another number so do the thing
					{
						for(int i = 0; i < spaceCount; i++)//give a whiteSpace per indention amount before putting operator on level
						{
							std::cout<<whiteSpace;
						}//end for
						std::cout<<character<<std::endl;//print out second item

						spaceCount -= 1; //decrease spaceCount
						opCount -= 1; //decrease opCount

						for(int i = 0; i < spaceCount; i++)//give a whiteSpace per indention amount before putting operator on level
						{
							std::cout<<whiteSpace;
						}//end for
						std::cout<<")"<<std::endl;
						parenCount--;
						prevChar = ")"; //assignment for number that follows lower level
					} //end second character check

					else
					{
						//spaceCount += 1; //item was operator so prepare for next level of operator
						//opCount += 1;
						holding.push_back(character); // Was not a number so put back on for further processing
					}
						
				}
				else // no items remain on the stack
					break;
				

				
				
			}// end else

			//std::cout<<character;
	
		}// end else if 0

		
	}//end while
	while(opCount > 0) // need to keep going back until opCount is zero
	{
		for(int i = 0; i < spaceCount; i++)//give a whiteSpace per indention amount before putting operator on level
			{
				std::cout<<whiteSpace;
			}//end for
			std::cout<<")"<<std::endl;
			
			opCount -= 1;
			spaceCount -= 1;
		
	}// end while opCount
	if(parenCount > 0)
		std::cout<<")"<<std::endl;
	

	//std::cout<<opCount<<std::endl;// do the last parenth

}// end dashLOption ************************************************************************************************************************


int main(int argc, char *argv[] )
{

std::vector<double> numHolding; // Vector to hold incoming strings from user
std::vector<std::string> charHolding; // vector to hold characters if -l option is used
std::string::size_type sz;
double stuff, firstNum, secondNum;
int tomFord = 0, errorOut = 0, dispOut = 0;

std::string nextChar = "bogusValue", whiteSpace = " ", thisChar = "nothing", dashL;

if(argc == 2) // one arguement given
	dashL = argv[1];
//std::cout<<dashL<<std::endl;
	while(1)
		{
			getline(std::cin, nextChar);
			
			charHolding.push_back(nextChar); // pushing everything in reverse order onto saved stack for later
			//result = isOperator(nextChar);
			//std::cout<<result<<std::endl;

			
			if(isOperator(nextChar) == 0) // The item is a number and can be pushed into vector
			{
				//std::cout<<"Into here";
				stuff = atof(nextChar.c_str()); // convert to double
				numHolding.push_back(stuff); // throw double into end of vector
			}//end 0 if

			else if(isOperator(nextChar) == 1) // item is addition
			{
				try
				{	if(numHolding.empty() == 0) // there is at least one item on stack
					{
						firstNum = numHolding.back();//get top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						secondNum = numHolding.back();//get new top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					
					firstNum = firstNum + secondNum;//add two numbers together
					numHolding.push_back(firstNum);//throw new number on top of stack
				}
				catch(...)
				{
					std::cout<<"Not enough items to perform operation";
				}
			}//end 1 else if

			else if(isOperator(nextChar) == 2) // item is subtraction
			{
				try
				{
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						firstNum = numHolding.back();//get top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						secondNum = numHolding.back();//get new top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					firstNum = firstNum - secondNum;//subtract two numbers with top of stack as first number
					numHolding.push_back(firstNum);//throw new number on top of stack
				}
				catch(...)
				{
					std::cout<<"Not enough items to perform operation";
				}
			}//end 2 else if

			else if(isOperator(nextChar) == 3) // item is multiplication
			{
				try
				{
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						firstNum = numHolding.back();//get top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						secondNum = numHolding.back();//get new top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					firstNum = firstNum * secondNum;//multiply two numbers together
					numHolding.push_back(firstNum);//throw new number on top of stack
				}
				catch(...)
				{
					std::cout<<"Not enough items to perform operation";
				}
			}//end 3 else if

			else if(isOperator(nextChar) == 4) // item is division
			{
				try
				{
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						firstNum = numHolding.back();//get top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						secondNum = numHolding.back();//get new top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}

					if(secondNum == 0)
					{
						std::cout<<"Error: Divide by zero"<<std::endl;
						dispOut = 1;
						tomFord = 1;
						break;
					}
					else
					{
						firstNum = firstNum / secondNum;//divide two numbers with top of stack as first number
						numHolding.push_back(firstNum);//throw new number on top of stack
					}
				}
				catch(...)
				{
					std::cout<<"Error: Divide by zero"<<std::endl;
					tomFord = 1; // Don't output result if given error
				}
			}//end 4 else if

			else if(isOperator(nextChar) == 5) // item is power to
			{
				try
				{
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						firstNum = numHolding.back();//get top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						secondNum = numHolding.back();//get new top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					firstNum = pow(firstNum, secondNum);// top of stack to the power of next on stack
					numHolding.push_back(firstNum);//throw new number on top of stack
				}
				catch(...)
				{
					std::cout<<"Not enough items to perform operation";
				}
			}//end 5 else if

			else if(isOperator(nextChar) == 6) // item is floored
			{
				try
				{
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						firstNum = numHolding.back();//get top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					firstNum = floor(firstNum);//Floor top item on stack
					numHolding.push_back(firstNum);//throw new number on top of stack
				}
				catch(...)
				{
					std::cout<<"Not enough items to perform operation";
				}
			}//end 6 else if

			else if(isOperator(nextChar) == 7) // item is ceilinged
			{
				try
				{
					if(numHolding.empty() == 0) // there is at least one item on stack
					{
						firstNum = numHolding.back();//get top item on stack
						numHolding.pop_back();//remove top item on stack
					}

					else // There is no items on stack, error to report
					{
						break;
					}
					firstNum = ceil(firstNum);//Ceil top item on stack
					numHolding.push_back(firstNum);//throw new number on top of stack
				}
				catch(...)
				{
					std::cout<<"Not enough items to perform operation";
				}
			}//end 7 else if
			
			else if(nextChar.empty()) //item is whitespace
				break; // Exit loop and return number

			else if(isOperator(nextChar) == 8) //item is none of the above
			{
				std::cout<<"Error: Invalid input '"<<nextChar<<"'"<<std::endl;
				tomFord = 1; // Don't output result if given error
				break;
			}
			
		}//end while

	if(tomFord == 0) // Did not get error
	{
		try
		{
		
			if(numHolding.empty())
			{
				std::cout<<"Error: Invalid number of inputs!"<<std::endl;
				dispOut = 1;
			}
			else
			{
				firstNum = numHolding.back();//should be final result
				numHolding.pop_back();//remove item from stack

				if(numHolding.empty())// if there is no remaining items on stack, then give answer
				{
					if(dashL.compare("-l") != 0) // only if no Dash L
					{
						std::cout<<"Result: " << firstNum<<std::endl;
						dispOut = 1;
					}
				}
				else // items remained on stack, give error
				{
					errorOut = 1; //Don't output items
				}
			}
		}
		catch(...)
		{
			std::cout<<"No items left on stack";
		}
	}// end tomFord check

	if(dashL.compare("-l") == 0 && errorOut == 0) // we have no error and dash L
	{
		if(tomFord == 0) //still no error, must be dash l option
		{
			dashLOption(charHolding); //do the dash and stash
		}
	}//end dash L check

	else // Error even in the face of dash L
		if(dispOut == 0) //Did not display result
			std::cout<<"Error: Invalid number of inputs!"<<std::endl;


}//end main *******************************************************************



int rpnProcessing(std::vector<std::string> &charHolding)
{
	int item;
	while(true)
	{
		std::string incoming = charHolding.front();
		item = isOperator(incoming); //check to see which thing it is
		
		if(item == 1)
		{
			
		}
	}
return 0;
}//end rpnProcessing














