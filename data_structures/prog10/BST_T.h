#ifndef BST_T_H
#define BST_T_H
/*--------------- B S T _ T . h --------------

PURPOSE
Define a binary search tree class template.

CHANGES
09-29-2018 gpc - Created for spring 2018
04-04-2018 ers - added RSearch, RInsert
04-26-2018 ers - added RRemoveLeaf(), RemoveLeaf(), and Empty(). Included CursorCntl
*/
#include <cassert>
#include <ostream>

using namespace std;

#include "CursorCntl.h"
//----- c l a s s    B S T -----


template <typename NodeData>
class BST
{
private:
   // Tree node class definition
   struct Node
   {
     // Constructors
	  Node() : left(0), right(0) {}
	  Node(const NodeData &d) : data(d), left(0), right(0) { }

	  // Data Members
     NodeData    data;    // The "contents" of the node
     Node        *left;   // Link to the left successor node
     Node        *right;  // Link to the right successor node
   };

public:
   // Constructor
   BST() : root(0), current(0) { }

   //True if tree is empty
   bool Empty() const {return root == 0;}

   // Search for an entry in the tree. If the entry is found,
   // make it the "current entry". If not, make the current entry 
   // NULL. Return true if the entry is found; otherwise return false. 
   bool Search(NodeData &d);
    
   // Add a new node to the tree.
   void Insert(NodeData &d);

   // Delete the current node.
   void Delete();

   //Remove the leftmost leaf node and return its data
   NodeData RemoveLeaf() { return RRemoveLeaf(root); }

   // Output the tree to the stream "os" in the indicated order.
   void OutputInOrder(ostream &os) const;    // Output inorder
   void OutputPreOrder(ostream &os) const;   // Output preorder
   void OutputPostOrder(ostream &os) const;  // Output postorder

   // Retrieve the data part of the current node.
   NodeData Current() { return current->data; }

   //Check to see if tree is empty
   bool Empty() { return root == 0; }

   // Show the binary tree on the screen. Only supported in Windows
   void ShowTree() const;
private:
   Node *root;      // Points to the root node
   Node *current;   // Points to the current node
   Node *parent;    // Points to current node's parent

   // Recursive Search
   bool RSearch(Node *subTree, NodeData &d);

   //Recursive remove the leftmost leaf node and return its data
   NodeData RRemoveLeaf(Node *&r);

   // Recursive Insert
   void RInsert(Node *&subTree, NodeData &d);

   // Recursive Traversal Functions
   void ROutputInOrder(Node *subTree, ostream &os) const;
   void ROutputPreOrder(Node *subTree, ostream &os) const;
   void ROutputPostOrder(Node *subTree, ostream &os) const;

   // Show the binary tree on the screen.
   void RShowTree(Node *subTree, int x, int y) const;
};


/*--------------- S e a r c h ( ) ---------------

PURPOSE
Search a BST for a specific data. If found
make that node the current node.

INPUT PARAMETERS
d - the item to look up

RETURN VALUE
true if found, otherwise false.
*/

template <typename NodeData>
bool BST<NodeData>::Search(NodeData &d)
{
	parent = 0;
	return RSearch(root, d);
}

/*--------------- I n s e r t ( ) ---------------

PURPOSE
Insert a node into the tree by calling Private recursive insert function

INPUT PARAMETERS
d - the item to insert
*/

template<typename NodeData>
void BST<NodeData>::Insert(NodeData &d)
{
	RInsert(root, d);
}

/*--------------- D e l e t e ( ) ---------------

PURPOSE
Delete the current node by calling Private recursive delete function
*/

template<typename NodeData>
void BST<NodeData>::Delete()
{
	//EXTRA CREDIT
	cout << "I didn't do this extra credit";
}

/*--------------- O u t p u t I n O r d e r ( ) ---------------

PURPOSE
Output the contents of the tree in alphabetical order by calling the respective Private recursive traversal function

INPUT PARAMETERS
os - the output stream the function outputs to
*/

template<typename NodeData>
void BST<NodeData>::OutputInOrder(ostream &os) const {
	ROutputInOrder(root, os);
}

/*--------------- O u t p u t P r e O r d e r ( ) ---------------

PURPOSE
Output the contents of the tree in preorder by calling the respective Private traversal function
INPUT PARAMETERS
os - the output stream the function outputs to
*/

template<typename NodeData>
void BST<NodeData>::OutputPreOrder(ostream &os) const {
	ROutputPreOrder(root, os);
}

/*--------------- O u t p u t P o s t O r d e r ( ) ---------------

PURPOSE
Output the contents of the tree in postorder by calling the respective Private traversal function

INPUT PARAMETERS
os - the output stream the function outputs to
*/

template<typename NodeData>
void BST<NodeData>::OutputPostOrder(ostream &os) const {
	ROutputPostOrder(root, os);
}


const int XRoot = 40;        // Column number for root node // gpc - 11-23-2017

/*--------------- R S h o w T r e e ( ) ---------------

PURPOSE
Recursive function to display a tree on the right half of the screen
using (crude) character graphics. Windows only

INPUT PARAMETERS
subTree   -- the root of the sub-tree to display
x         -- the cursor x coordinate index
y         -- the cursor y coordinate
*/
template <typename NodeData>
void BST<NodeData>::RShowTree(Node *subTree, int x, int y) const
{
  const int VertSpacing = 7;   // Vertical spacing constant // gpc - 11-23-2017
  const int HorizSpacing = 10; // Horizontal spacing of tree nodes // gpc - 11-23-2017
  const int MaxLevels = 4;     // The number of levels that fit on the screen // gpc - 11-23-2017

  // If the tree is not empty display it..
  if (subTree != 0 && x < MaxLevels) // gpc - 11-23-2017
    {
    // Show the left sub-tree.
    RShowTree(subTree->left, x+1, y+VertSpacing/(1<<x));

    // Show the root.
    gotoxy(XRoot+HorizSpacing*x, y);
	 subTree->data.Show(cout);
	 cout << endl;

    // Show the right subtree.
    RShowTree(subTree->right, x+1, y-VertSpacing/(1<<x));
    }
}

/*--------------- S h o w T r e e ( ) ---------------

PURPOSE
Display a tree on the right half of the screen using (crude)
character graphics.This function calls RShowTree() which does
the work.  Windows only
*/
template <typename NodeData>
void BST<NodeData>::ShowTree() const
{
  const int YRoot = 11;      // Line number of root node // gpc - 11-23-2017
  const int ScrollsAt = 24;  // Screen scrolls after line 24 // gpc - 11-23-2017
    
  int xOld;                       // Old cursor x coordinate
  int yOld;                       // Old cursor y coordinate

  // Save cursor position
  getxy(xOld, yOld);

  // Has the screen scrolled yet?
  int deltaY = 0;

  if (yOld > ScrollsAt)
    deltaY = yOld - ScrollsAt+1;

  // Clear the right half of the screen.
  for (int y=0; y<25; y++)
    {
    gotoxy(XRoot,y);
    clreol();
    }
  // Show the tree and offset if scrolled.
  RShowTree(root, 0, YRoot+deltaY);   

  // Restore old cursor position.
  gotoxy(xOld,yOld);      
}

/*--------------- R S e a r c h ( ) ---------------

PURPOSE
Recursively search a BST for a specific data. If found
make that node the current node.

INPUT PARAMETERS
*subTree - the subtree to be searched
d - the item to look up

RETURN VALUE
true if found, otherwise false.
*/

template <typename NodeData>
bool BST<NodeData>::RSearch(Node *subTree, NodeData &d)
{
	if (subTree == 0) {
		current = 0;
		return false;
	}
	else if (subTree->data == d) {
		current = subTree;
		return true;
	}
	else if (d < subTree->data)
		return RSearch(subTree->left, d);
	else
		return RSearch(subTree->right, d);
}


/*--------------- R I n s e r t ( ) ---------------

PURPOSE
Recursively insert a node into the tree.

INPUT PARAMETERS
*subTree - the subtree to be into
d - the item to insert
*/

template <typename NodeData>
void BST<NodeData>::RInsert(Node *&subTree, NodeData &d){
	if (subTree == 0) {
		Node* newNode = new(nothrow) Node(d);
		subTree = newNode;
		assert(newNode != 0);
	}
	else if (d < subTree->data)
		RInsert(subTree->left, d);
	else if (d > subTree->data)
		RInsert(subTree->right, d);
	else {
		subTree->data.Update();
	}

}


/*--------------- R O u t p u t I n O r d e r ( ) ---------------

PURPOSE
Recursively traverse the tree in alphabetical order

INPUT PARAMETERS
*subTree - the subTree to be traversed
os - the output stream the function outputs to
*/

template<typename NodeData>
void BST<NodeData>::ROutputInOrder(Node *subTree, ostream &os) const {
	if (subTree != 0) {
		ROutputInOrder(subTree->left, os);
		os << subTree->data.Word() << "=" << subTree->data.Count() << endl;
		ROutputInOrder(subTree->right, os);
	}
}

/*--------------- R O u t p u t P r e O r d e r ( ) ---------------

PURPOSE
Output the contents of the tree in preorder recursively

INPUT PARAMETERS
*subTree - the subTree to be traversed
os - the output stream the function outputs to
*/

template<typename NodeData>
void BST<NodeData>::ROutputPreOrder(Node *subTree, ostream &os) const {
	if (subTree != 0) {
		os << subTree->data.Word() << "=" << subTree->data.Count() << endl;
		ROutputPreOrder(subTree->left, os);
		ROutputPreOrder(subTree->right, os);
	}
}

/*--------------- R O u t p u t P o s t O r d e r ( ) ---------------

PURPOSE
Output the contents of the tree in postorder recursively

INPUT PARAMETERS
*subTree - the subtree to be traversed
os - the output stream the function outputs to
*/

template<typename NodeData>
void BST<NodeData>::ROutputPostOrder(Node *subTree, ostream &os) const {
	if (subTree != 0) {
		ROutputPostOrder(subTree->left, os);
		ROutputPostOrder(subTree->right, os);
		os << subTree->data.Word() << "=" << subTree->data.Count() << endl;
	}
}

/*--------------- R R e m o v e L e a f ( ) ---------------

PURPOSE
Remove the leftmost leaf node and delete it from the tree

INPUT PARAMETERS
r - the root node of the subtree from which the leaf node is to be removed

RETURN VALUE
the data part of the removed leaf
*/

template<typename NodeData>
NodeData BST<NodeData>::RRemoveLeaf(Node *&r)
{
	//Make sure the tree is not empty
	assert(r != 0);

	//Does this node have any successors?
	if (r->left != 0)
		//There is a left successor, traverse left subtree
		return RRemoveLeaf(r->left);
	else if (r->right != 0)
		//There is a right successor, traverse right subtree
		return RRemoveLeaf(r->right);
	else 
	{
		//There are no successors, it's a leaf node, capture node data
		NodeData result = r->data;

		//Delete the leaf node
		delete r;

		//Mark the subtree empty
		r = 0;

		//Return the data from the removed node
		return result;
	}
}
#endif