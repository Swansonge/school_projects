/*--------------- L i n k e d L i s t . c p p ---------------

by:   George Cheney
      16.322 Data Structures
      ECE Department
      UMASS Lowell

PURPOSE
This is an array-based list module.

CHANGES
03-06-2018 gpc - Created for 16.322
03-21-2018 ers - updated Insert and Delete functions, added Skip function
*/
#include "LinkedList.h" 

/*--------------- I n s e r t ( ) ---------------

PURPOSE
Insert a new entry before the current entry

INPUT PARAMETERS
d  -- the data to insert into the list
*/
void List::Insert(const NodeData &d)
{

	//if inserting at the beginning of the list
	if (pred == 0){
		Node* newNode = new(nothrow) Node(d, first);
		assert(newNode != 0);
		first = newNode;
		pred = newNode;
		//pred->next = current;
	}
	//if the inserting into middle of list
	else
	{
		Node* newNode = new(nothrow) Node(d, pred->next);
		assert(newNode != 0);
		pred->next = newNode;
		pred = newNode;
	}

	assert(!Empty());
}

/*--------------- D e l e t e ( ) ---------------

PURPOSE
Delete the current entry from the list.
*/
void List::Delete(void)
{
  assert(!Empty());

  //if deleting first node in the list
  if (pred == 0)
  {
	  first = current->next;
	  delete current;
	  current = first;
  }
  //if deleting node from middle of list
  else
  {
	  pred->next = current->next;
	  delete current;
	  current = pred->next;
  }
}

/*--------------- S k i p ( ) ---------------

PURPOSE
Skip to the next entry in the list
*/

void List::Skip()
{
	assert(!AtEnd());
	pred = current;
	current = current->next;
}

