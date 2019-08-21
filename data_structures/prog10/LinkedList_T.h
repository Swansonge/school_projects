/*---------------L i n k e d L i s t . h ---------------

by:   George Cheney
      16.322 Data Structures
      ECE Department
      UMASS Lowell

PURPOSE
This header file is the interface to the module "List.cpp."

CHANGES
03-06-2018 gpc - Created for 16.322
03-19-2018 ers - Updated the class to be a linked list
04-26-2018 ers - Added Update() function
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <cassert>


//---------- c l a s s  L i n k e d L i s t ----------

template<typename ItemData>
class List
{
private:
	//List node class definition
	struct Node 
	{
		ItemData data; //The "contents" of the node
		Node *next; //Link to the next node

		//Node Constructor Functions
		Node() {}
		Node(const ItemData &theData, Node *const theNext = 0) : data(theData), next(theNext) {}
	};
public:
	//Constructor
	List() : first(0), current(0), pred(0) {}

	//True if the list is empty
	bool Empty() const { return first == 0; }

	//True if the current position is beyond the last entry.
	bool AtEnd() const {return current == 0;}

	//Rewind the current entry to the beginning of the list
	void Rewind() { current = first; pred = 0; }

	//Skip to the next entry in the list
	void Skip();

	//Get the contents of the current lost entry.
	ItemData CurrentEntry() const
	{
		assert(!AtEnd());
		return current->data;
	}

	//Insert a new list entry before the current entry
	void Insert(const ItemData &d);

	//Update the current entry.
	void Update(const ItemData &d) { assert(!AtEnd()); current->data = d; }

	//Delete the current entry.
	//The new current entry is the successor of the deleted node.
	void Delete();

private:
	Node *first; //Points to the first node in the list
	Node *current; //Points to the current node
	Node *pred; //Points to the node preceding the current entry
};

/*--------------- I n s e r t ( ) ---------------

PURPOSE
Insert a new entry before the current entry

INPUT PARAMETERS
d  -- the data to insert into the list
*/
template<typename ItemData>
void List<ItemData>::Insert(const ItemData &d)
{

	//if inserting at the beginning of the list
	if (pred == 0) {
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

template<typename ItemData>
void List<ItemData>::Delete(void)
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

template<typename ItemData>
void List<ItemData>::Skip()
{
	assert(!AtEnd());
	pred = current;
	current = current->next;
}


#endif