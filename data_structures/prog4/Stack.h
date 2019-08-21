#ifndef STACK_H
#define STACK_H

#include <cassert>
using namespace std;

#include "Position.h"

const int StackCapacity = 100;

/*--------------- S t a c k . h ---------------

by:   George Cheney
EECE.3220 Data Structures
ECE Dept.
UMASS Lowell

PURPOSE
maintain an array based stack

DEMONSTRATES
Stack as an array

CHANGES
10-11-2017 gpc - Create for EECE.3220 class.
*/

// Make the stack element type be a grid position.
typedef Position StackElement;

//---------- c l a s s    S t a c k ----------

// Define a linked list based stack class.

class Stack
{

	//Stack node class definition
	struct Node
	{
		StackElement data; //The "contents" of the node
		Node *next; //Link to the next node
		//Node constructor functions
		Node() {}
		Node(const StackElement &theData, Node *const theNext = 0)
			:data(theData), next(theNext) {}
	};
public:
	// Contructor
	Stack() : tos(0) {};

	// Return true if stack is empty
	bool Empty() const { return tos == 0; }

	// Push a new element onto the top of the stack.
	void Push(const StackElement &elem);

	// Retrieve the top element and pop it off of the stack.
	StackElement Pop();

	// Retrieve the top element, but do not remove it from the stack.
	StackElement Top() const;
private:
	Node *tos;
};
#endif

