/**********************************************
Eric Swnanson
Header file for integration assignment
***********************************************/

// Lines used for conditional compilation--ensures header isn't included multiple times
#ifndef prog5_functions_h
#define prog5_functions_h

// Calculate integral of f(x) over interval min to max, using n trapezoids
double integrate(double min, double max, int n);

// f(x) as defined in the program specification
double f(double x);

// Used to clear line if input formatting error occurs
void badInput();

#endif /* More conditional compilation--end of prog5_functions_h */
