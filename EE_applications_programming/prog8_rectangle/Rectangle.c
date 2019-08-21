//Eric Swanson
//first written
//completed 4/19/17
//These functions are the functions that deal with the rectangle-based functions. One prints a rectangle,
//one prints an array of rectangles, one returns the area of a specified rectangle, one returns the perimeter
//of a rectangle, one will tell the user if two specified rectangles overlap

#define _CRT_SECURE_NO_WARNINGS
#include "Rectangle.h"		// Implicitly includes Point.h
#include <stdio.h>

// Print contents of rectangle
// Prints vertices in appropriate relative positions:
//   vert[1]   vert[2]
//   vert[0]   vert[3]
void printRectangle(Rectangle *r) {
	printPoint(&r->vert[1]);
	printf("   ");
	printPoint(&r->vert[2]);
	printf("\n");
	printPoint(&r->vert[0]);
	printf("   ");
	printPoint(&r->vert[3]);
}

// Print list of n Rectangles
void printList(Rectangle list[], int n) {
	int i; //loop variable
	for (i = 0;i < n;i++) {
		printRectangle(&list[i]);
		printf("\n\n");
	}
}

// Returns area of rectangle
double area(Rectangle *r) {
	return (dist(&r->vert[3], &r->vert[0])*dist(&r->vert[1], &r->vert[0]));
}

// Returns perimeter of rectangle
double perimeter(Rectangle *r) {
	return (dist(&r->vert[1], &r->vert[0]) + dist(&r->vert[2], &r->vert[1]) + dist(&r->vert[3], &r->vert[2]) + dist(&r->vert[0], &r->vert[3]));
}

// Returns 1 if two rectangles overlap; 0 otherwise
int overlap(Rectangle *r1, Rectangle *r2) {
	//if outside the points exist outside rectangle return 0. Else, return 1
	if (r2->vert[2].x  < r1->vert[1].x || r2->vert[1].x > r1->vert[2].x) {
		return 0;
	}
	else if (r2->vert[2].y  < r1->vert[0].y || r2->vert[0].y > r1->vert[2].y) {
		return 0;
	}
	else
		return 1;
}
