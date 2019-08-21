/*--------------- S t a c k . c p p ---------------

by:   George Cheney
EECE.3220 Data Structures
ECE Dept.
UMASS Lowell

PURPOSE
Maintain an array based stack

DEMONSTRATES
Stack as an array

CHANGES
10-11-2017 gpc - Create for EECE.3220 class.
*/
#include "stack.h"

/*--------------- P u s h ( ) ---------------

Push a new element onto the top of the stack.

INPUT PARAMETERS
elem  -- the element to add to the top of stack
*/
void Stack::Push(const StackElement &elem)
{
	tos = new(nothrow) Node(elem, tos);
	assert(!Empty());

}

/*--------------- P o p ( ) ---------------

Pop the top element off of the stack and return its value.

RETURN VALUE
The old top of stack
*/
StackElement Stack::Pop()
{
		Node *oldTos = tos; //current top of stack to be deleted
		StackElement bringBack = tos->data; //used to return data of the top of stack
		tos = tos->next;
		delete oldTos;
		return bringBack;
}

/*--------------- T o p ( ) ---------------

Returns the value of the data on top of the stack

RETURN VALUE
The top of stack
*/
StackElement Stack::Top() const
{
	assert(!Empty());

	return tos->data;
}
