
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include "prog5_functions.h"

// Calculate integral of f(x) over interval min to max, using n trapezoids
double integrate(double min, double max, int n) {
	double width; //total width of interval
	double delta_x; //width of each trapezoid
	int k;
	double y0; 
	double h1;
	double h2;
	double sum;
	double yn;
	double xCount;
	double area; //area under function/ the value of the integral


	width = max - min; 
	delta_x = width / n; 
	y0 = f(min);
	h1 = y0;
	xCount = min + delta_x;
	h2 = f(xCount);
	yn = f(max);
	sum = 0;
	//calculate sum of yk
	for (k = 1; k <= (n - 1); k++) {
		sum += h2;
		h1 = h2;
		xCount += delta_x;
		h2 = f(xCount);
		
	}
	//area calculation
	area = 0.5*delta_x*(y0 + (2 * sum) + yn);
	return area;
	
}

// f(x) as defined in the program specification
double f(double x) {
	double func;

	func = sin(x) + (x*x) / 10;
	return func;
}

// Used to clear line if input formatting error occurs
void badInput() {
	char junk;

	do {
		scanf("%c", &junk);
	} while (junk != '\n');
}