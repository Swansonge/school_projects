/* Eric Swanson
First Written: 4/2/17
Edited: 4/5/7
Finished: 4/6/17
* Implements simple raster scan graphics program-- will print box and grid, line by line Tests use of 2-D arrays,
nested loops, strings.  A two-dimensional array will be used to store a grid and a
series of boxes will be added to that grid using string-based commands.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "prog7_functions.h"


int main() {
	char myGrid[NRows][NCols];
	char cmd[6]; //orignally empty string used for entered comman
	char prnt[] = "print"; //print string, compared to user input
	char rst[] = "reset"; //reset string, compared to user input
	char add[] = "add"; //add string, compared to user input
	char ext[] = "exit"; //exit string, compared to user input
	int cont; //value to determine if function will continue or not
	int x; //x coordinate of origin entered by user
	int y; //y coordinate of origin entered by user
	int width; //width of box entered by user
	int height; //height of box entered by user
	int xVals; //checks value of input entered for x and y
	int nVals; //checks value of input entered for width and height

	resetGrid(myGrid);			// Initialize grid to clear state

	do {
		printf("\nEnter a command (add, print, reset, exit): ");
		scanf("%s", cmd);
		cont = 1;
		//print condition
		if (strcmp(cmd, prnt) == 0) {
			printGrid(myGrid);
			cont = 1;
			badInput();
		}
		//reset condition
		else if (strcmp(cmd, rst) == 0) {
			resetGrid(myGrid);
			cont = 1;
			badInput();
		}
		//add condition
		else if (strcmp(cmd, add) == 0) {
			do {
			printf("Enter X and Y coordinates for origin: ");
			xVals = scanf("%d %d", &x, &y);
				if (xVals != 2) {
					printf("Error: incorrect input\n");
					badInput();
				}
				else {
					do {
					printf("Enter width and height: ");
					nVals = scanf("%d %d", &width, &height);
						if (nVals != 2) {
							printf("\nError: incorrect input\n");
							badInput();
						}
						else {
							addBox(myGrid, x, y, width, height);
						}
					} while (nVals != 2);
				}
				cont = 1;
			} while (xVals != 2);
		}
		//exit condition
		else if (strcmp(cmd, ext) == 0) {
			return 0;
		}
		//improper input condition
		else {
			printf("Error: incorrect input\n");
			badInput();
			cont = 1;
		}
	} while (cont == 1);
}