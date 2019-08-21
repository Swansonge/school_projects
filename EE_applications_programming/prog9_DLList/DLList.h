/*************************************************
16.216: ECE Application Programming
UMass Lowell
M. Geiger
Header file for 9th programming assignment
Contains structure definitions, function prototypes

********DO NOT MODIFY THIS FILE ********

**************************************************/

// Structure definitions
// Node in doubly-linked list
typedef struct DLN {
	struct DLN *prev;		// Pointer to previous list element
	struct DLN *next;		// Pointer to next list element
	char *word;
} DLNode;

// Actual doubly-linked list
typedef struct {
	DLNode *firstNode;	// Pointer to first node in list
	DLNode *lastNode;	// Pointer to last node in list
} DLList;

// Function prototypes
DLNode *findNode(DLList *list, char *str);		// Find node containing str
void addNode(DLList *list, char *str);			// Add new node to list containing str
void delNode(DLList *list, char *str);			// Delete node containing str from list
void printList(DLList *list);					// Print contents of list