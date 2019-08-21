#ifndef QUEUE_H
#define QUEUE_H

/*--------------- Q u e u e T . h ---------------

by:   George Cheney
      EECE.3220 Data Structures
      ECE Dept.
      UMASS Lowell

PURPOSE
Define a queue class template

CHANGES
09-29-2018 gpc - Created for spring 2018
*/

#include <cassert>

template <typename ElemData>
class Queue
{
private:
   // List node class definition
   struct Node
   {
      ElemData    data;    // The "contents" of the node
      Node        *pred;   // Link to the predecessor node
      Node        *succ;   // Link to the successor node

      // Node Constructor Functions
      Node()
         {
         assert(this != 0);
         pred = 0;
         succ = 0;
         }
      Node(ElemData &theData, Node *prd = 0)
         {
         assert(this != 0);
         data = theData;
         pred = prd;
         succ = 0;
         }
   };

public:
   // Queue Operations
	Queue() : front(0), back(0) { }
   bool Empty() const { return front == 0; }
   void EnQueue(ElemData &p);
   ElemData DeQueue();
	ElemData Front();
private:
   Node *front;
	Node *back;
};

/*--------------- E n Q u e u e ( ) --------------

PURPOSE
Add an item to the back of the queue.

INPUT PARAMETER
p -- the item to add
*/

template <typename ElemData>
void Queue<ElemData>::EnQueue(ElemData &p)
{
   Node *newNode;
   
   try
      {
	   newNode = new Node(p,back);
	   }
	catch (bad_alloc ex)
	   {
	   cerr << "\n***ERROR: new Failure at Line" << __LINE__ - 4 << " in File " << __FILE__ << endl;
	   exit(EXIT_FAILURE);
	   }

//	assert(newNode != 0);

   if (front == 0)
		{
		back = newNode;
		front = back;
		}
	else
		{
      back->succ = newNode;
	   back = newNode;
		}
}

/*--------------- D e Q u e u e ( ) --------------

PURPOSE
Remove the from item from the queue.

*/
template <typename ElemData>
ElemData Queue<ElemData>::DeQueue()
{
   assert(!Empty()); 

	ElemData	frontData = front->data;

	Node *oldFront = front;

	front = front->succ;

	delete oldFront;

	if (front != 0)
		front->pred = 0;
	else
		back = 0;

   return frontData;
}

template <typename ElemData>
ElemData Queue<ElemData>::Front()
{
   assert(!Empty()); 

   return front->data;
}

#endif
