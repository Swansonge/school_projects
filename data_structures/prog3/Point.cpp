//=============== Point . c p p ===============

#include "Point.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cassert>

using namespace std;

const unsigned MinValue = 0; //minimum allowed value for a coordinate


//----- C l a s s    P o i n t   M e m b e r  F u n c t i o n s -----

/*-----  X ( ) -----

PURPOSE: extract the x value member of the point

*/

double Point::X() {

	//coordinate must not be negative
	assert(defined);
	return x;
}

/*-----  Y ( ) -----

PURPOSE: extract the y value member of the point

*/

double Point::Y() {

	//coordinate must not be negative
	assert(defined);
	return y;
}

/*-----  D e f i n e d ( ) -----

PURPOSE: extract the defined member of the point

*/

bool Point::Defined() {

	return defined;

}

/*-----  S e t ( ) -----

PURPOSE: set the x and y values for the class object

INPUT PARAMETERS:
xVal -- the x coordinate value
yVal -- the y coordinate value
*/

void Point::Set(double xVal, double yVal) {

	assert(xVal >= MinValue);
	assert(yVal >= MinValue);

	x = xVal;
	y = yVal;
	defined = true;
}

/*-----  L e n g t h ( ) -----

PURPOSE: Compute the length of a line connecting two points: the calling point and "p2"

INPUT PARAMETERS:
pt2 -- ending point of a line
*/

double Point::Length(Point p2) {

	double dx = p2.x - x;
	double dy = p2.y - y;

	return sqrt(dx*dx + dy*dy);
}

/*-----  G e t ( ) -----

PURPOSE: Receive input from an input stream for the location of the point. Default value is cin

INPUT PARAMETERS:
&is -- the location of the input stream
*/

void Point::Get(istream &is) {

	//Mark point undefined
	defined = false;

	//if not an empty line, read it in
	if (is.peek() != '\n') {
		//read in the x and y coordinates
		is >> x >> y;
		//mark the point defined
		defined = true;
	}

	is.ignore(INT_MAX, '\n');

	return;
}

/*-----  S h o w ( ) -----

PURPOSE: Display the location of the point to an output stream. Default value is cout

INPUT PARAMETERS:
&os -- the location of the output stream
*/

void Point::Show(ostream &os) {

	//make sure point has been defined
	assert(defined);

	os << "(" << x << ", " << y << ")" << endl;
}
