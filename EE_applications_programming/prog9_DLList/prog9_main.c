/*************************************************
16.216: ECE Application Programming
UMass Lowell
M. Geiger
Main program for 9th programming assignment
Working with dynamic memory allocation

********DO NOT MODIFY THIS FILE ********

**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DLList.h"				// User-defined header file

char *readLine();				// Reads string from input and dynamically allocates space

void main() {
	DLList myList = { NULL, NULL };	// Doubly-linked list used throughout program
									// Since list is empty, firstNode and lastNode are
									//    both initialized to NULL
	char *cmd = NULL;				// Input command
	char *word = NULL;				// Input word

	do {
		printf("\nEnter command:  ");
		cmd = readLine();		// Read string from standard input

								// Add word to list
		if (strcmp(cmd, "add") == 0) {
			printf("Enter word to be added:  ");
			word = readLine();
			addNode(&myList, word);
		}

		// Delete word from list
		else if (strcmp(cmd, "delete") == 0) {
			printf("Enter word to be deleted:  ");
			word = readLine();
			delNode(&myList, word);
		}

		// Find word in list
		else if (strcmp(cmd, "find") == 0) {
			printf("Enter word to be found:  ");
			word = readLine();
			if (findNode(&myList, word) == NULL)
				printf("%s not found in list\n", word);
			else
				printf("%s found in list\n", word);
		}

		// Print contents of entire list
		else if (strcmp(cmd, "print") == 0)
			printList(&myList);

		// Invalid command
		else if (strcmp(cmd, "exit") != 0)
			printf("Invalid command %s\n", cmd);

	} while (strcmp(cmd, "exit") != 0);
}

// Reads string from standard input and dynamically allocates space
// Assumes string terminates with '\n'
char *readLine() {
	int i = 0;			// Loop index
	char *inStr;		// Input string
	char *tempStr;		// Temp string
	char inCh;			// Input character
	i = 0;

	inStr = (char *)malloc(1);	// Start with 1-character string
	if (inStr == NULL) {
		fprintf(stderr, "Could not allocate\n");
		return NULL;
	}

	// Repeatedly read input characters and reallocate space to store string
	while ((inCh = fgetc(stdin)) != '\n') {
		inStr[i++] = inCh;
		tempStr = (char *)realloc(inStr, (i + 1) * sizeof(char));

		// Reallocation failed
		if (tempStr == NULL) {
			fprintf(stderr, "Could not reallocate\n");
			return inStr;
		}

		// Reallocation successful; assign tempStr to inStr
		else inStr = tempStr;
	}

	inStr[i] = '\0';
	return inStr;
}