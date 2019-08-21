/*--------------- P o i n t . c p p ---------------

by:   George Cheney
      ECE Department
      UMASS Lowell

PURPOSE
Define operations on class "Point."

CHANGES:
03-06-2018 gpc - Created for 16.322
*/

#include <climits>

#include "Point.h"

/*---------- P o i n t : : G e t ( ) -----

PURPOSE
Input a point from the stream "is."

INPUT PARAMETERS
is    -- the stream from which the point is to be read
*/
void Point::Get(istream &is)
{
   // Check for empty input.
   if (is.peek() == '\n')
      // Empty, mark the point undefined.
      defined = false;
   else
      {
      // Not empty. Read the x and y components and mark defined.
      is >> x >> y;
      defined = true;
      }

   // Flush to beginning of next line.
   is.ignore(INT_MAX, '\n');
}

/*---------- P o i n t : : S h o w ( ) -----

PURPOSE
Output a point to the stream "os."

INPUT PARAMETERS
os    -- the stream to which the point is to be written
*/
void Point::Show(ostream &os)
{
   // Make sure that the point exists.
   assert(defined);

   // Display the point.
   os << "(" << x << ", " << y << ")";
}

/*===== M U T A T O R S =====*/

/*--------------- P o i n t : : S e t ( ) ---------------

PURPOSE
Set the coordinates of a point:

  (x, y) = (xVal, yVal)

*/
void Point::Set(double xVal, double yVal)
{
   x = xVal;
   y = yVal;

   defined = true;
}

/*--------------- P o i n t : : L e n g t h ( ) ---------------

PURPOSE
Compute the length of the line from the start point to the end point.
*/
double Point::Length(Point pt2)
{
   // x distance from start to end
   double dx = pt2.x - x;
   // y distance from start to end
   double dy = pt2.y - y;

   // Length of line.
   return sqrt(dx*dx + dy*dy);
}
