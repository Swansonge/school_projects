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
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <cassert>

#include "Point.h"

typedef Point NodeData;

//---------- c l a s s  L i n k e d L i s t ----------

class List
{
private:
	//List node class definition
	struct Node 
	{
		NodeData data; //The "contents" of the node
		Node *next; //Link to the next node

		//Node Constructor Functions
		Node() {}
		Node(const NodeData &theData, Node *const theNext = 0) : data(theData), next(theNext) {}
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
	NodeData CurrentEntry() const
	{
		assert(!AtEnd());
		return current->data;
	}

	//Insert a new list entry before the current entry
	void Insert(const NodeData &d);

	//Update the current entry.
	void Update(const NodeData &d) { assert(!AtEnd()); current->data = d; }

	//Delete the current entry.
	//The new current entry is the successor of the deleted node.
	void Delete();
private:
	Node *first; //Points to the first node in the list
	Node *current; //Points to the current node
	Node *pred; //Points to the node preceding the current entry
};

#endif