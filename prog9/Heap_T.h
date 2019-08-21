#ifndef HEAP_H
#define HEAP_H
/*--------------- H e a p _ T . h ---------------

by:   George Cheney
EECE322 Data Structures
ECE Department
UMASS Lowell

PURPOSE
Implements standard heap operations including Heap Sort.

DEMONSTRATES
Heaps and Heap Sort

CHANGES
04-05-2018 gpc - Created for EECE322
04-17-2018 ers - added Heap function definitions
04-18-2018 ers - added Heap function definitions
04-19-2018 ers - finished function definitions
*/

#include <iostream>

#include <cassert>
#include "CursorCntl.h"
template <typename ElemData, unsigned Capacity>
class Heap
{
public:
	// Constructor
	Heap() : size(0), current(0) { }
	// Return the number of elements in the array.
	unsigned Size() { return size; }
	// Return true if the array is empty.
	bool Empty() { return size == 0; }
	// Return true if the array is full.
	bool Full() { return size >= Capacity; }
	// Insert a new element into the array properly stored in ascending order..
	void Insert(ElemData &data);
	// Perform Heap Sort to sort the array into ascending order.
	void Sort();
	// Call "BinSearch()" to search the sorted array for the entry "data". 
	// If found, make this the current entry and return true;
	// otherwise, return false.
	bool Search(ElemData &data);

	// Perform a binary search for "data". Search the index range from
	// "start" to "end". If the item is found, make it the current item and return true.
	// Otherwise, return false.
	bool BinSearch(unsigned start, unsigned end, ElemData &data);

	// Output the array to the stream "os".
	void Output(ostream &os);
	// Show the heap on the right side of the screen.
	void ShowTree() const;
	// Return the current entry.
	ElemData CurrentEntry() { return heap[current]; }
   // Update the current entry.
   void Update() { assert(current != 0); heap[current].Update(); }

	// Standard heap operations
	void PercolateUp();
	void DeleteMax();
	void PercolateDown(unsigned r, unsigned n);
	void Heapify();

private:
	unsigned size;				// The number of items in the heap
	unsigned current;			// The index of the entry found by the last search
	ElemData heap[Capacity+1];	// The heap array

	// Recursive function to show the tree graphics
	void RShowTree(unsigned r, int x, int y) const;
};

const unsigned XRoot = 40;        // Column number for root node

/*--------------- R S h o w T r e e ( ) ---------------

PURPOSE
Recursive function to display a tree on the right half of the screen
using (crude) character graphics.

INPUT PARAMETERS
r         -- the root of the sub-tree to display
x         -- the cursor x coordinate index
y         -- the cursor y coordinate
*/
template <typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::RShowTree(unsigned r, int x, int y) const
{
  const unsigned VertSpacing = 7;   // Vertical spacing constant
  const unsigned HorizSpacing = 10; // Horizontal spacing of tree nodes
  const unsigned MaxLevels = 4;     // The number of levels that fit on the screen

  // If the tree is not empty display it..
  if (r <= size && x < MaxLevels)
    {
    // Show the left sub-tree.
    RShowTree(2*r, x+1, y+VertSpacing/(1<<x));

    // Show the root.
    gotoxy(XRoot+HorizSpacing*x, y);

	 ElemData wc = heap[r];

	 wc.Show(cout);

    // Show the right subtree.
    RShowTree(2*r+1, x+1, y-VertSpacing/(1<<x));
    }
}

/*--------------- S h o w T r e e ( ) ---------------

PURPOSE
Display a tree on the right half of the screen using (crude)
character graphics.This function calls RShowTree() which does
the work.
*/
template <typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::ShowTree() const
{
  const unsigned YRoot = 12;      // Line number of root node
  const unsigned ScrollsAt = 24;  // Screen scrolls after line 24

#if (defined _WIN32) && (!defined NoGraphics)

  int xOld;                       // Old cursor x coordinate
  int yOld;                       // Old cursor y coordinate

  // Save cursor position
  getxy(xOld, yOld);

  // Show the tree and offset if scrolled.
  RShowTree(1, 0, YRoot);   

  // Restore old cursor position.
  gotoxy(xOld,yOld);      
#endif
}

/*--------------- I n s e r t ( ) ---------------

PURPOSE
Insert a new element into the array properly stored in ascending order

INPUT PARAMETERS
data -- the data that's inserted into the array

*/
template <typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::Insert(ElemData &data)
{
	assert(!Full());
	size++; //add item as new last node
	heap[size] = data;
	PercolateUp(); 
}

/*--------------- S o r t ( ) ---------------

PURPOSE
Perform Heap Sort to sort the array into ascending order

*/
template <typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::Sort()
{

	Heapify(); //convert array to heap
	unsigned n = size;

	//repeat until heap only has index 0
	while (n != 0)
	{
		ElemData store = heap[n];
		heap[n] = heap[1];
		heap[1] = store;

		PercolateDown(1, --n);
	}
}


/*--------------- S e a r c h ( ) ---------------

PRUPOSE
Call "BinSearch()" to search the sorted array for the entry "data".
If found, make this the current entry and return true;
otherwise, return false;

INPUT PARAMETERS
data - the data you're searching for

OUTPUT PARAMETERS
true/false - true if data found, false if data not found
*/
template <typename ElemData, unsigned Capacity>
bool Heap<ElemData, Capacity>::Search(ElemData &data)
{
	return !Empty() && BinSearch(1, size, data); //automatically not found if empty
}


/*--------------- B i n S e a r c h ( ) ---------------

PURPOSE
Perform a recursive binary search for "data". Search the index range from "start" to "end". If found, make it the current item and return true. Otherwise, return false.

INPUT PARAMETERS
start - index from which the search is started
end -  index at which the search ends
data - the data you're searching for

OUTPUT PARAMETERS
true/false - true if data found, false if data not found
*/
template <typename ElemData, unsigned Capacity>
bool Heap<ElemData, Capacity>::BinSearch(unsigned start, unsigned end, ElemData &data)
{
	unsigned mid = start + (end - start) / 2; //find middle of array
	if (start > end)
		return false;
	if (data == heap[mid])
	{
		current = mid;
		return true;
	}
	else if (data > heap[mid])
		return BinSearch(mid + 1, end, data);
	else
		return BinSearch(start, mid - 1, data);
}

/*--------------- O u t p u t ( ) ---------------

PURPOSE
Output the array to the stream "os"

INPUT PARAMETERS
os - the output stream
*/

template<typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::Output(ostream &os)
{
	for (unsigned i = 1; i <= size; i++) {
		heap[i].Show(os);
		os << endl;
	}
	return;
}

/*--------------- P e r c o l a t e U p ( ) ---------------

PURPOSE
swap branch nodes with parent nodes until parent is not smaller
*/
template<typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::PercolateUp()
{
	unsigned loc = size; //index of last node
	unsigned parent = loc / 2; //parent of loc

	//Repeat if not at root and parent is smaller
	while (parent >= 1 && heap[loc] > heap[parent])
	{
		ElemData store = heap[parent];
		heap[parent] = heap[loc];
		heap[loc] = store;

		loc = parent; //move up to parent node
		parent = loc / 2; //new parent node
	}
}

/*--------------- D e l e t e M a x ( ) ---------------

PURPOSE
Delete the top of the heap/root (greatest-valued node)
*/
template<typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::DeleteMax()
{
	assert(!Empty());
	heap[1] = heap[size];
	PercolateDown(1, --size);
}

/*--------------- P e r c o l a t e D o w n ( ) ---------------

PURPOSE
repeatedly exchange value from root with its larger child until there is no larger children

INPUT PARAMETERS
r - the root node
n - the last node
*/

template<typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::PercolateDown(unsigned r, unsigned n)
{
	unsigned c = 2 * r; //find left child of r
	while (c <= n)
	{
		if (c < n && heap[c] < heap[c + 1]) //pick larger child
			c++;
		if (heap[r] < heap[c]) //if child is bigger, swap
		{
			ElemData store = heap[c];
			heap[c] = heap[r];
			heap[r] = store;

			r = c; //this is the nect child root
			c = 2 * c; //next left child
		}
		else
			return; //child is not bigger, done
	}
}

/*--------------- H e a p i f y ( ) ---------------

PURPOSE
convert any left-complete binary tree to a heap
*/
template<typename ElemData, unsigned Capacity>
void Heap<ElemData, Capacity>::Heapify()
{
	for (unsigned r = size / 2; r >= 1; r--)
		PercolateDown(r, size);
	return;
}
#endif