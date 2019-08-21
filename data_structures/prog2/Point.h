//== == == == == == == = Point.h == == == == == == == =
#ifndef POINT_H
#define POINT_H


// --------------- D a t a    T y p e    P o i n t ---------------

//Define the location of a point in Cartesian coordinates
struct Point {
	double x; //x-coordinate of the point
	double y; //y-coordinate of the point
	bool defined; //true if point already defined; otherwise false
};

//----- D a t a    T y p e    P o i n t    F u n c t i o n  P r o t o t y p e s-----

double Length(Point pt1, Point pt2);

void GetPoint(Point &pt);

void ShowPoint(Point pt);

#endif