//
//  Point.h
//  prog8_rectangles
//
//  Created by Michael Geiger on 4/12/16.
//  Copyright © 2016 Michael Geiger. All rights reserved.
//

#ifndef Point_h
#define Point_h

typedef struct {
	double x;		// X coordinate
	double y;		// Y coordinate
} Point;

// Print coordinates as (x.xx, y.yy)
void printPoint(Point *p);

// Read input coordinates
void readPoint(Point *p);

// Return distance between two points
double dist(Point *p1, Point *p2);

#endif /* Point_h */
