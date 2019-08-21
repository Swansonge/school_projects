//== == == == == == == = Point.h == == == == == == == =

#include <iostream>
#include <cassert>
using namespace std;

#ifndef POINT_H
#define POINT_H

// ----------- C l a s s  P o i n t ----------

//class Point describes a point in cartesian coordinates with various member functions
class Point
{
public:

	//Constructors 
	Point() : x(0), y(0), defined(false) {} //Default constructor
	Point::Point(double xVal, double yVal) : x(xVal), y(yVal), defined(true) //Explicit constructor
	{
		assert(xVal >= 0);
		assert(yVal >= 0);
	}

	//Accessors
	double X();
	double Y();
	bool Defined();

	//Mutators
	void Set(double xVal, double yVal);

	//Length of a line from calling point to pt2
	double Length(Point pt2);

	//Input-Output
	void Get(istream &is= cin);
	void Show(ostream &os= cout);

private:
	double x; //x coordinate
	double y; //y coordinate
	bool defined; //true if this point has been defined; otherwise false
};

#endif
 