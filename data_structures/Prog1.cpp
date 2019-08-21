/*=============== P r o g 1 . c p p ===============

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
*/

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// --------------- D a t a    T y p e    P o i n t ---------------

//Define the location of a point in Cartesian coordinates
struct Point {
	double x; //x-coordinate of the point
	double y; //y-coordinate of the point
	bool defined; //true if point already defined; otherwise false
};

//----- D a t a    T y p e    P o i n t    F u n c t i o n s -----

/*-----  L e n g t h ( ) -----

PURPOSE: Compute the length of a line connecting two points "pt1" and "pt2"

INPUT PARAMETERS:
pt1 -- starting point of a line
pt2 -- ending point of a line
*/

double Length(Point pt1, Point pt2) {
	double xDif; //difference between x points
	double yDif; //difference between y points
	double xSqr; //difference in x squared
	double ySqr; //difference in y squared
	double sumSqr; //sum of sthe above squares
	double len; //length

	xDif = pt2.x - pt1.x;
	yDif = pt2.y - pt1.y;

	xSqr = xDif * xDif;
	ySqr = yDif * yDif;

	sumSqr = xSqr + ySqr;
	len = sqrt(sumSqr);

	return len;
}

/*-----  G e t P o i n t ( ) -----

PURPOSE: Read from the keyboard a pair of doubles (x,y) giving the location of a point.

INPUT PARAMETERS:
pt -- point to be returned
*/

void GetPoint(Point &pt) {
	cin >> pt.x;

	//if point negative, mark as undefined
	if (pt.x < 0) {
		pt.defined = false;
		return;
	}
	else {
		cin >> pt.y;
		pt.defined = true;
	}
}

/*-----  S h o w P o i n t ( ) -----

PURPOSE: Display the point "pt" on the screen in Cartesian form i.e."(x,y)"

INPUT PARAMETERS:
pt -- point to be displayed.
*/

void ShowPoint(Point pt) {

	//display point if defined
	if (pt.defined) 
		cout << "(" << pt.x << ", " << pt.y << ")" << endl;
	else 
		cout << "The point is not defined";

}


// --------------- D a t a    T y p e    P o l y g o n ---------------

//Define a convex polygon
const unsigned MaxSides = 10; //Maximum allowed sides in a polygon
const unsigned MinSides = 3; //A valid polygon has at least 3 sides

struct Polygon {
	unsigned numSides; //Number of sides in the polygon
	Point v[MaxSides]; //The points giving the polygon vertexes
};


//----- D a t a    T y p e    P o l y g o n    F u n c t i o n s -----

/*-----  P o l y C i r c u m f e r e n c e ( ) -----

PURPOSE: Determine the circumference of a given polygon

INPUT PARAMETERS:
thePoly -- polygon whose circumference is to be obtained
*/

double PolyCircumference(Polygon &thePoly) {
	double cirCount = 0; //running sum of circumference
	unsigned i; //loop variable

	//find the length between the consecutive points and add to cirCount
	for (i=0;i < thePoly.numSides - 1; i++) {
		cirCount += Length(thePoly.v[i], thePoly.v[i + 1]);
	}

	//length between end point and start point
	cirCount += Length(thePoly.v[i], thePoly.v[0]);
	
	return cirCount;
}

/*-----  P o l y A r e a ( ) -----

PURPOSE: Determine the value of the area of the polygon as well as the areas of which it is composed.

INPUT PARAMETERS:
thePoly -- polygon whose area is to be obtained of the polygon and the subsequent elements give the area of the i'th triangle in the polygon
area -- address of an array in which element 0 gives the total area
*/

void PolyArea(Polygon &thePoly, double *area) {
	double a; //length of side a of the triangle
	double b; //length of side b of the triangle
	double c; //length of side c of the triangle
	double s; //sum of a,b,and c, divided by 2
	unsigned numTriangle; //number of triangles (n-2 where is number of sides of polygon)
	unsigned i; //loop variable

	//first triangle only
	area[0] = 0;
	area[1] = 0;
	a = Length(thePoly.v[0], thePoly.v[1]); //length from poly 1st point (triangle start point) to poly 2nd point (triangle 2nd point)
	b = Length(thePoly.v[1], thePoly.v[2]); //length from poly 2nd point to poly 3rd point (triangle end point)
	c = Length(thePoly.v[2], thePoly.v[0]); //length from poly 3rd point to poly 1st point
	s = (a + b + c) / 2;
	area[1] = sqrt(s*(s - a)*(s - b)*(s - c)); //add area for first triangle as second element of area[]
	area[0] = area[0] + area[1]; //add the first triangle to the total polygon area in element 0 of area[]

	//if more than one triangle, calculate area of other triangles starting at poly 3rd point
	numTriangle = thePoly.numSides - 2;
	if (numTriangle > 1) {
		for (i = 2; i < thePoly.numSides - 1; i++) {
			a = Length(thePoly.v[0], thePoly.v[i]); //length from poly 1st point (triangle start point) to poly point i+1 (triangle 2nd point)
			b = Length(thePoly.v[i], thePoly.v[i + 1]); //length from poly point i+1 to poly point i+2 (triangle end point)
			c = Length(thePoly.v[i + 1], thePoly.v[0]); //length from poly point i+2 to poly 1st point
			s = (a + b + c) / 2;

			area[i] = sqrt(s*(s - a)*(s - b)*(s - c)); //add area for single triangle as an element of area[]
			area[0] = area[0] + area[i]; //add the triangle to the total polygon area in element 0 of area[]
		}
	}


}


/*--------------- S h o w P o l y ( ) ----------

PURPOSE: Display a polygon.

INPUT PARAMETERS:
p  -- the polygon to be displayed.
*/
void ShowPoly(Polygon &p)
{
	for (unsigned i = 0; i<p.numSides; i++)
		ShowPoint(p.v[i]);
}

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
