//Eric Swanson
//first written
//completed 4/19/17
//These functions deal with the point values of the rectangles. One prints specified coordinates for a point,
//one reads user inputs for coordinates for the point, one calculates and returns the distance betwwen two points

#define _CRT_SECURE_NO_WARNINGS
#include "Point.h"
#include <stdio.h>
#include <math.h>

// Print coordinates as (x.xx, y.yy)
void printPoint(Point *p) {
	printf("(%.2lf, %.2lf)", p->x, p->y);
}

// Read input coordinates
void readPoint(Point *p) {
	scanf("%lf %lf", &p->x, &p->y);
}

// Return distance between two points
double dist(Point *p1, Point *p2) {
	return sqrt(((p2->x - p1->x)*(p2->x - p1->x)) + ((p2->y - p1->y)*(p2->y - p1->y)));
}