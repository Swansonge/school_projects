#ifndef HASH_TABLE
#define HASH_TABLE
/*--------------- H a s h T a b l e . h ---------------

by:   George Cheney
      16.322 Data Structures
      ECE Department
      UMASS Lowell

PURPOSE
This header file is the interface to the module "HashTable.cpp."

CHANGES
11-28-2017 gpc -  Created for 16.322
11-30-2017 gpc -  Don't have CurrentEntry() return a reference.
11-30-2017 gpc -  Add Update function to update the current entry in the table.
12-08-2017 gpc -  Change bucket[] from array of pointers to linked lists to
                  array of linked lists.
04-13-2018 gpc -  Update for spring 2018.
04-26-2018 ers - Wrote Definitions for member functions
*/

#include <iostream>
#include <iomanip>

using namespace std;

#include "CursorCntl.h"
#include "LinkedList_T.h"

//----- c l a s s    H a s h T a b l e

template <typename ItemData, unsigned NumBuckets>
class HashTable
{
public:
	//Constructor: Make all buckets empty.
	HashTable() { size = 0; }

   // Search the table for entry "d." If "d" is found, return true,make "d" 
   // the current entry for this bucket, and make this the current bucket.
   // If "d" is not found, return false.
   bool Search(ItemData &d);
   // Add an entry to the table.
   void Insert(ItemData &d);
   // Update an entry in the table.
   void Update(ItemData &d)
   {
      unsigned index = Hash(d);		      // Compute bucket index.
      bucket[index].Update(d);	// Update the current entry in this bucket.
   }

   // Delete the current entry.
   void Delete();
   // Return true if the table is empty.
   bool Empty() { return size == 0; }
   // Return the number of entries in the table.
   unsigned Size() { return size; }
   // Return the current entry.
   ItemData CurrentEntry();
   // Remove the first entry from the table.
   ItemData Remove();
   // Show the hash table on the right side of the screen.
   void Show();
private:
   unsigned size;       // The number of entries in the table.
   unsigned curBucket;  // The index of the current bucket
   List<ItemData> bucket[NumBuckets]; // Array of pointers to linked lists
   //ItemData current; //current item in the table
   // The hashing function
   unsigned long Hash(ItemData &d);
};

/*--------------- H a s h ( ) ---------------

PURPOSE
Obtain a hash key from the ItemData object "d" and then generate
a random table index such that 0 <= index <= Num Buckets-1.

INPUT PARAMETERS
d  -- the ItemData object from which to obtain a hash key
*/
template <typename ItemData, unsigned NumBuckets>
unsigned long HashTable<ItemData, NumBuckets>::Hash(ItemData &d)
{
   // Ask ItemData "d" for its hash key.
   unsigned x = d.HashKey();

   // Now, generate a random table index such that
   //      0 <= index <= NumBuckets-1
   const unsigned C1 = 25173;
   const unsigned C2 = 13849;
   const unsigned C3 = 65536;

   return ((C1*x + C2) % C3) % NumBuckets;
}


/*--------------- S h o w ( ) ---------------

PURPOSE
Display a the hash table on the right half of the screen.
*/
template <typename ItemData, unsigned NumBuckets>
void HashTable<ItemData, NumBuckets>::Show()
{
#ifndef NoGraphics
	const unsigned XLeft = 40;          // Column number for start of dictionary display
	const unsigned XHeading = XLeft - 3;// Column location of heading
	const unsigned ScrollsAt = 24;      // Screen scrolls after line 24
	const unsigned XMax = 79;           // Don't show words after this column
	const unsigned YSpacing = NumBuckets < 22 ? 22 / NumBuckets : 1;  // Vertical spacing
	const unsigned DisplayLines = NumBuckets < 22 ? NumBuckets : 22;// Number of buckets to display

	int xOld;                       // Old cursor position x coordinate
	int yOld;                       // Old cursor position y coordinate

									// Save cursor position
	getxy(xOld, yOld);

	// Has the screen scrolled yet?
	int deltaY = 0;

	if (yOld > ScrollsAt)
		deltaY = yOld - ScrollsAt + 1;

	// Clear the right half of the screen.
	for (int y = 0; y<ScrollsAt + 1; y++)
	{
		gotoxy(XLeft, y + deltaY);
		clreol();
	}

	// Display heading.
	gotoxy(XHeading, deltaY);
	cout << "BUCKET";

	// Show the array and offset if scrolled.
	for (unsigned index = 0; index<DisplayLines; index++)
	{
		// Display the bucket number.
		gotoxy(XLeft, YSpacing*index + deltaY + 2);
		cout << setw(2) << right << index << ": ";

		// Traverse the linked list bucket,
		// displaying each entry.
		bucket[index].Rewind();
		while (!bucket[index].AtEnd())
		{
			int xCursor;  // cursor x position
			int yCursor;  // cursor y position

							// Don't go off the right side of the screen
			getxy(xCursor, yCursor);
			if (xCursor + bucket[index].CurrentEntry().Word().length() >= XMax)
				break;

			// Display the next entry from the bucket. 
			cout << left;
			bucket[index].CurrentEntry().Show();
         cout << " ";
			bucket[index].Skip();
		}
	}

	// Restore old cursor position.
	gotoxy(xOld, yOld);
#endif  
}

/*--------------- S e a r c h ( ) ---------------

PURPOSE
Search the table for the entry "d." If "d" is found, return true, make "d"
the current entry for this bucket, and make this the current bucket. If "d" is not found, return false

INPUT PARAMETERS
d  -- the ItemData object to be searched for 

RETURN VALUE
true/false - true if item found, false if item not found
*/

template<typename ItemData, unsigned NumBuckets>
bool HashTable<ItemData, NumBuckets>::Search(ItemData &d)
{
	if (Empty())
		return false;
	unsigned long key = Hash(d);
	//bucket corresponding to item's hash key. Check for item in here
	List<ItemData> *check = &bucket[key];
	check->Rewind();
	if (check->Empty())
		return false;
	//search through entire bucket
	while (!check->AtEnd())
	{
		if (check->CurrentEntry() == d)
		{
			curBucket = key;
			return true;
		}
		check->Skip();
	}
	return false;
}

/*--------------- I n s e r t ( ) ---------------

PURPOSE
Add an entry to the table

INPUT PARAMETERS
d  -- the ItemData object to be inserted

*/

template<typename ItemData, unsigned NumBuckets>
void HashTable<ItemData, NumBuckets>::Insert(ItemData &d)
{
	unsigned long key = Hash(d);
	bucket[key].Insert(d);
	size++;
	return;
}

/*--------------- D e l e t e ( ) ---------------

PURPOSE
Delete the current entry

*/

template<typename ItemData, unsigned NumBuckets>
void HashTable<ItemData, NumBuckets>::Delete()
{
	bucket[curBucket].Delete();
	size--;
}

/*--------------- C u r r e n t E n t r y ( ) ---------------

PURPOSE
Return the current entry

RETURN VALUE
the current entry
*/

template<typename ItemData, unsigned NumBuckets>
ItemData HashTable<ItemData, NumBuckets>::CurrentEntry()
{
	return bucket[curBucket].CurrentEntry();
}

/*--------------- R e m o v e  ( ) ---------------

PURPOSE
Remove the first entry from the table

RETURN VALUE
the first entry in the table
*/

template<typename ItemData, unsigned NumBuckets>
ItemData HashTable<ItemData, NumBuckets>::Remove()
{
	unsigned count = 0;
	while (bucket[count].Empty())
		count++;
	//rewind the table's first bucket. That bucket's current entry is the first entry in the table
	bucket[count].Rewind();
	ItemData firstEntry = bucket[count].CurrentEntry();

	//delete the first entry and return its value
	bucket[count].Delete();
	size--;
	return firstEntry;
}
#endif