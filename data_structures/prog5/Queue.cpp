/*--------------- Q u e u e . c p p ---------------

by:   George Cheney
      16.322 Data Structures
      ECE Dept.
      UMASS Lowell

PURPOSE
This module defines an array based queue class.

CHANGES
10-22-2017 gpc - Create for EECE322 class.
10-23-2017 gpc - update comments.
03-01-2018 gpc - Update for spring 2018
*/

using namespace std;

#include <cassert>

#include "queue.h"


/*--------------- E n q u e u e ( ) --------------

PURPOSE
Add an item to the tail of the queue.

INPUT PARAMETER
elem -- the item to add
*/

void Queue::Enqueue(QueueElem &p)
{
	if (!Empty()) {
		
		Node *newNode;
		newNode = new(nothrow) Node(p);
		tail->succ = newNode;
		tail = newNode;
	}
	else if (Empty()) {
		Node *newNode;
		newNode = new(nothrow) Node(p);
		head = newNode;
		tail = newNode;
	}
	assert(!Empty());
}

		/*--------------- D e q u e u e ( ) --------------

PURPOSE
Remove and return the head item from the queue.

*/
QueueElem Queue::Dequeue()
{
	QueueElem elem = head->data;
	if (head == tail) {
		delete head;
		head = 0;
		tail = 0;
	}
	else {
		Node *newHead;
		newHead = head->succ;
		delete head;
		head = newHead;
	}

   // Return the old head data.
   return elem;
}


/*--------------- H e a d ( ) --------------

PURPOSE
Return but do not remove the head item from the queue.

*/
QueueElem Queue::Head()
{
   // Insure not empty.
  assert(!Empty());

   // Return the head item data.
  return head->data;
}
