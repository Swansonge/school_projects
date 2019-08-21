/*
* 16.216 ECE Application Programming
* Starter file by M. Geiger to
*   raster graphics programming assignment
* Functions edited by Eric Swanson
* Source file containing function definitions
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "prog7_functions.h"

// Reset grid to original settings
void resetGrid(char myGrid[][NCols]) {
	int i; //index for array rows
	int j; //index for array columns

	for (i = NRows - 1; i >= 0; i--) {
		for (j = 0; j < NCols; j++) {
			if (i % 5 != 0 && j % 5 == 0) {
				myGrid[i][j] = '|';
			}
			else if (j % 5 != 0 && i % 5 == 0) {
				myGrid[i][j] = '-';
			}
			else if (i % 5 == 0 && j % 5 == 0) {
				myGrid[i][j] = '+';
			}
			else {
				myGrid[i][j] = ' ';
			}
		}
	}
}

// Add box to existing grid starting at (x, y) with specified width & height
void addBox(char myGrid[][NCols], int x, int y, int width, int height) {
	int i; //index for array rows
	int j; //index for array columns

		for (i = y; i < (y + height); i++) {
			for (j = x; j < (x + width); j++) {
				if (i >= 0 && j >= 0 && i <= NRows && j <= NCols) {
					myGrid[i][j] = '*';
				}
			}
		}
}

// Print current grid contents
void printGrid(char myGrid[][NCols]) {
	int i; //index for array rows
	int j; //index for array columns

	printf("\n");
	for (i = NRows - 1; i >= 0; i--) {
		for (j = 0; j < NCols; j++) {
			printf("%c", myGrid[i][j]);
		}
		if (i % 5 == 0) {
			printf("%d\n", i);
		}
		else {
			printf("\n");
		}
	}
	printf("0    5    10   15   20   25   30   35   40   45   50\n\n");
}

// Used to clear line if input formatting error occurs
void badInput() {
	char junk;

	do {
		scanf("%c", &junk);
	} while (junk != '\n');
}
