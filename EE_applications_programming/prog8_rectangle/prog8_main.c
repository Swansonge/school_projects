//
//  main.c
//  prog8_rectangles
//
//  Created by Michael Geiger on 4/12/16.
//  Copyright © 2016 Michael Geiger. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Rectangle.h"      // Includes "Point.h" as well

int main() {
	Rectangle rlist[10];    // Array containing up to 10 rectangles
	int nRect = 0;			// Number of rectangles
	char cmd;               // Single character command
	int ind, ind2;			// Loop index
	int flag;				// Flag to simplify conditionals
	int nread;				// Number of inputs read
	char junk;				// Used to clear input

							// Loop until user enters 'Q'
	while (1) {
		printf("\nEnter command <A | P | D | O | Q>: ");
		scanf(" %c", &cmd);

		switch (cmd) {

			// Add rectangle
		case 'A': case 'a':
			if (nRect == 10)		// List is full
				printf("No room in list of rectangles\n");
			else {
				printf("Enter coordinates as x y, starting with lower left hand corner:\n");
				readPoint(&rlist[nRect].vert[0]);
				readPoint(&rlist[nRect].vert[1]);
				readPoint(&rlist[nRect].vert[2]);
				readPoint(&rlist[nRect].vert[3]);
				nRect++;
			}
			break;

			// Print list
		case 'P': case 'p':
			if (nRect == 0)		// List is empty
				printf("No rectangles in list\n");
			else
				printList(rlist, nRect);
			break;

			// Print dimensions
		case 'D': case 'd':

			// Ask for index into array and validate input
			do {
				printf("Enter index into array: ");
				nread = scanf("%d", &ind);
				if (nread == 0) {
					printf("Format error for index input\n");
					do {
						scanf("%c", &junk);
					} while (junk != '\n');
				}
				else if ((ind < 0) || (ind >= nRect))
					printf("Invalid index %d\n", ind);
			} while ((nread == 0) || (ind < 0) || (ind >= nRect));

			// Actually print dimensions of desired rectangle
			printf("Area of rectangle %d: %.2lf\n", ind, area(&rlist[ind]));
			printf("Perimeter of rectangle %d: %.2lf\n", ind, perimeter(&rlist[ind]));
			break;

			// Test for overlap of two rectangles
		case 'O': case 'o':

			// Prompt for indices into array and validate input
			// Uses a slightly different method for looping
			//   than we've used before (set a "flag" to show
			//   error has occurred and only test that flag
			//   at the end of each loop iteration)
			do {
				flag = 0;
				printf("Enter indices to test: ");
				nread = scanf("%d %d", &ind, &ind2);
				if (nread < 2) {
					printf("Format error for indices input\n");
					do {
						scanf("%c", &junk);
					} while (junk != '\n');
					flag = 1;
				}
				else {
					if ((ind < 0) || (ind >= nRect)) {
						printf("Invalid index %d\n", ind);
						flag = 1;
					}
					if ((ind2 < 0) || (ind2 >= nRect)) {
						printf("Invalid index %d\n", ind2);
						flag = 1;
					}
				}
			} while (flag == 1);

			// Use overlap function to test for overlap
			if (overlap(&rlist[ind], &rlist[ind2]))
				printf("Rectangles %d and %d overlap\n", ind, ind2);
			else
				printf("Rectangles %d and %d do not overlap\n", ind, ind2);

			break;


			// Exit program
		case 'Q': case 'q':
			return 0;

		default:
			printf("Invalid command %c\n", cmd);
		}
	}
	return 0;
}