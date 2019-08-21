/*************************************************
Eric Swanson
First Written: 4/26/17
Completed: 4/28/17
These functions are the functions that realize the creation of a doubly-linked list
(a common data structure) using dynamic memory allocation and structures. The aforementioned 
structures are of two types: a node with pointers pointing to the previous and next node in a list, and 
data in the form of a string (a word); and, a list containing these nodes--this will ultimately be a list
of words. The functions in this program have different jobs; one adds a word to the list (using the node structures),
one finds a specified word in the list, one deletes a word from the list, and one prints the list of words in
alphabetical order. The list is sorted alphabetically.

**************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "DLList.h"

// Find node containing word
// Return pointer to node, or NULL if nothing found
DLNode *findNode(DLList *list, char *str) {
	DLNode *n = list->firstNode;
	while ((n != NULL) && strcmp(n->word, str) <= 0) {
		if (strcmp(n->word, str) == 0) {
			return n;
		}
		n = n->next;
	}
	return NULL;
}

// Add new node to list containing str
void addNode(DLList *list, char *str) {
	DLNode *cur=list->firstNode; //node for current node that checks nodes in list
	DLNode *newNode = (DLNode *)malloc(sizeof(DLNode)); //new node to be added to list
	newNode->word = str;
	//if list is empty
	if (list->firstNode == NULL && list->lastNode == NULL) {
		list->firstNode = newNode;
		list->lastNode = newNode;
		newNode->prev = NULL;
		newNode->next = NULL;
	}
	//if new node is beginning of list
	else if (strcmp(str, list->firstNode->word) <= 0) {
		newNode->prev = NULL;
		newNode->next = list->firstNode;
		list->firstNode->prev = newNode;
		list->firstNode = newNode;
	}		
	
	else {
		while ( cur != NULL && strcmp(cur->word, str) <= 0) {
			cur = cur->next;
		}
		//new node is in middle of list
		if (cur != NULL) {
			newNode->prev = cur->prev;
			newNode->next = cur;
			cur->prev->next = newNode;
			cur->prev = newNode;
		}
		//if new node is end of list
		else if (cur == NULL) {
			newNode->prev = list->lastNode;
			newNode->next = NULL;
			list->lastNode->next = newNode;
			list->lastNode = newNode;
		}
	}

		
}

// Delete node containing word from list if it is present
void delNode(DLList *list, char *str) {
	DLNode *cur = list->firstNode; //node for current node that checks nodes in list
	//trying to delete from empty list
	if ((list->firstNode == NULL) && (list->lastNode == NULL)) {
		printf("the list is empty, nothing to be deleted\n");
		return NULL;
	}
	//if word was found and there is only one word in list
	else if ((list->firstNode == list->lastNode) && strcmp(list->firstNode->word, str) == 0) {
		free(cur);
		list->firstNode = NULL;
		list->lastNode = NULL;
	}
	//if word was found in the beginning
	else if (strcmp(list->firstNode->word, str) == 0) {
		cur = list->firstNode->next;
		cur->prev = NULL;
		free(list->firstNode);
		list->firstNode = cur;
	}
	//if word was found in the end
	else if (strcmp(list->lastNode->word, str) == 0) {
		cur = list->lastNode->prev;
		cur->next = NULL;
		free(list->lastNode);
		list->lastNode = cur;
	}
	//search until you find item or hit end
	else {
		DLNode *found;
		found = findNode(list, str);
		if (found == NULL) {
			printf("word is not in the list\n");
		}
		else if (strcmp(found->word, str)==0) {
			found->next->prev = found->prev;
			found->prev->next = found->next;
			free(found);
		}
	}
}


// Print contents of list, or message
void printList(DLList *list) {
	DLNode *n = list->firstNode;
	if (list->firstNode == NULL && list->lastNode == NULL) {
		printf("\nList is empty\n");
	}
	else {
		printf("\nContents of list:\n");
		while (n != NULL) {
			printf("%s\n", n->word);
			n = n->next;
		}
	}
}