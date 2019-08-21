/*--------------- P r o g 3 . c p p ---------------

Starter Code by:  George Cheney
ECE Dept.
UMASS Lowell

Finshed Code by:  Your Name Here

PURPOSE
Read in the definition of an arbitrary convex polygon, and then
compute both the circumerence and the area of the polygon.

DEMONSTRATES
C++ CLASSES
Accessor Member Functions
Mutator Member Functions
Constructor Member Functions
Input/Output Member Functions

CHANGES
02-11-2018 gpc -  Published to EECE.3220.
02-13-2018 ers - added Polygon struct, Point class, and the Polygon and Point functions
02-14-2018 ers - modern style constructor functions
*/

#include <iostream>
using namespace std;

#include "Point.h"
#include "Polygon.h"


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
		Point nextPt;
		nextPt.Get(cin);
		if (!nextPt.Defined())
			break;
		poly.v[poly.numSides] = nextPt;
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

	// Add one more point to the polygon.
	cout << "\nNow add one more point: ";

	double x;   // x and y coordinates of extra point
	double y;

	// Read in the x and y coordinates.
	cin >> x >> y;
	// Use the Point constructor function to combine x and y.
	Point newPt(x, y);
	// Use the Point accessors to display the x and y coordinates of the added point.
	cout << "\nHere is the added point: " << "(" << newPt.X() << ", " << newPt.Y() << ")" << endl;
	// Append the new point to the polygon.
	poly.v[poly.numSides++] = newPt;
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
