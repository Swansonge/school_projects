#ifndef QUEUE_H
#define QUEUE_H

/*--------------- Q u e u e . h ---------------

by:   George Cheney
      16.322 Data Structures
      ECE Dept.
      UMASS Lowell

PURPOSE
This is the interface to Queue.cpp.

CHANGES
10-22-2017 gpc - Create for EECE322 class.
10-23-2017 gpc - Update comments.
03-01-2018 gpc - Update for spring 2018
*/

#include "Position.h"

// Queue.h

typedef Position  QueueElem;  // The queue stores maze row/columns positions.


//--------------- C l a s s    Q u e u e ---------------

//Define a generic queue class.

class Queue
{
private:
	//List node class definition
	struct Node 
	{
		QueueElem data; //The "contents" of the node
		Node *succ; //Link the successor node

		Node() : succ(0) {}
		Node(const QueueElem &theData) : data(theData), succ(0) {}
	};

public:
	Queue() : head(0), tail(0) {} //default constructor
	bool Empty() const { return head == 0; } //returns true if empty
	void Enqueue(QueueElem &p); //add new tail element
	QueueElem Dequeue(); //remove and return head element
	QueueElem Head(); //return but dont remove head element
private:
	Node *head; //points to head node
	Node *tail; //points to tail node
};
#endif
