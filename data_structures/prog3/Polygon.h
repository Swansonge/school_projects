//== == == == == == == = Polygon.h == == == == == == == =

#ifndef POLYGON_H
#define POLYGON_H
#include "Point.h"

// --------------- D a t a    T y p e    P o l y g o n ---------------

//Define a convex polygon
const unsigned MaxSides = 10;
const unsigned MinSides = 3;

struct Polygon {
	unsigned numSides; //Number of sides in the polygon
	Point v[MaxSides]; //The points giving the polygon vertexes
};

//----- D a t a    T y p e    P o l y g o n    F u n c t i o n  P r o t o t y p e s-----

double PolyCircumference(Polygon &thePoly);

void PolyArea(Polygon &thePoly, double *area);

void ShowPoly(Polygon &p);

#endif
