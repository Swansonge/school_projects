/*=============== P r o g 2 . c p p ===============

Starter Code by:  George Cheney
ECE Dept.
UMASS Lowell

Finshed Code by:  Eric Swanson

PURPOSE
Read in the definition of an arbitrary convex polygon, and then
compute both the circumerence and the area of the polygon.

CHANGES
01-24-2018 gpc -  Updated for EECE322 class.
01-30-2018 updated the code for the functions. Defined structs.
02-03-2018 terminate on empty line functionally. Added Point.h, Polygon.h, Point.cpp, Polygon.cpp
*/

#include "Point.h"
#include "Polygon.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;



//--------------- m a i n ( ) ---------------

int main()
{
	Polygon poly;   // The polygon definition

					// Start out with zero polygon sides.
	poly.numSides = 0;

	// Read in a polygon definition. If a valid polygon was entered,
	// display its circumference and area; otherwise display an
	// error message and terminate execution.
	cout << "ENTER A POLYGON DEFINITION: " << endl << endl;

	for (;;)
	{
		// Read in the next point
		cout << "Enter next point: ";
		GetPoint(poly.v[poly.numSides]);
		if (!poly.v[poly.numSides].defined)
			break;

		// Update the ploygon size.
		++poly.numSides;

		// If the polygon is full, say so and stop taking input.
		if (poly.numSides >= MaxSides)
		{
			cout << "\nPolynomial Full" << endl;
			break;
		}
	}

	// Make sure that the polygon is valid.
	if (poly.numSides < MinSides)
	{
		cout << "***ERROR: A polygon must have at least 3 sides" << endl;
		return 0;
	}

	// The definition is valid, show the definition.
	cout << endl << "Here is the polygon definition:" << endl;
	ShowPoly(poly);

	// Compute and display the circumference.
	cout << "\nCircumference = " << PolyCircumference(poly) << endl;

	// Compute and display the areas of the polygon and its embedded triangles.

	// area[0] gives the area of the entire polygon.
	// area[i] (i > 0) gives the area of the i'th embedded triangle.
	double area[MaxSides + 1];

	PolyArea(poly, area);

	cout << "\nPolygon Area = " << area[0] << endl << endl;

	const unsigned numTriangles = poly.numSides - 2;   // Number of embedded triangles

	for (unsigned i = 1; i <= numTriangles; ++i)
		cout << "Triangle " << i << ": Area = " << area[i] << endl;

	cout << endl;

	return 0;
}
