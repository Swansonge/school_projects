//=============== Polygon . c p p ===============

#include "Polygon.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

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
	for (i = 0;i < thePoly.numSides - 1; i++) {
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