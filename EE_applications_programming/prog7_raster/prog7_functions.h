/*
* 16.216 ECE Application Programming
* Starter file by M. Geiger to
*   raster graphics programming assignment
* Header file containing function prototypes
*/

#ifndef prog7_functions_h
#define prog7_functions_h

#define NRows 21	// Dimensions for grid
#define NCols 51

// Reset grid to original settings
void resetGrid(char myGrid[][NCols]);

// Add box to existing grid starting at (x, y) with specified width & height
void addBox(char myGrid[][NCols], int x, int y, int width, int height);

// Print current grid contents
void printGrid(char myGrid[][NCols]);

// Used to clear line if input formatting error occurs
void badInput();

#endif /* prog7_functions_h */
