//=============== Point . c p p ===============

#include "Point.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

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
	
	//check for new line to make when user done entering numbers

	if (cin.peek() == '\n') {
		//If empty input line, mark the point undefined
		pt.defined = false;
	}
	else {
		//otherwise read point and mark as defined
		cin >> pt.x >> pt.y;
		pt.defined = true;
	}

	//discard characters until new line discarded
	cin.ignore(INT_MAX, '\n');

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


