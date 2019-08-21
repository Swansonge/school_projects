/*Eric Swanson
Written : 2 / 23 / 17
The goal of this program is to approximate the integral of a function, f(x), using the trapezoidal rule.
Two functions will written to do this (written as header files), and these functions will be inserted into
the main C program; One function calculates the integral, one function defines the function to be integrated,
f(x), and the last function clears the line if their is a formatting error.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "prog5_functions.h"

int main() {
	double min, max; //min and max values for the interval
	int n; //number of trapezoids used in integration
	int mVals; //variable to read content of scanf() for min and max
	int nVals; //variable to read content of scanf() for number of trapezoids
	double ans; //answer returned by integration
	int error; //checks if incorrect answer to yes or no question after integral calculation
	char Ans; //answer for the yes or no question
	char check; //checks for value of scanf() of yes or no answer question after integral calculation
	int cont; //keeps track if the program will run again or not

	//Ask and read input for min and max values
	do {
		do {
			printf("Enter endpoints of interval to be integrated (low hi): ");
			mVals = scanf("%lf %lf", &min, &max);
			if (mVals != 2) {
				printf("Error: incorrect input format\n");
				badInput();
			}
			else if (min > max) {
				printf("Error: the low value cannot be greater than the hi value\n");
				badInput();
			}
		} while (mVals != 2 || min > max);

		//Ask and read input for number of trapezoids
		do {
			printf("Enter the number of trapezoids to be used: ");
			nVals = scanf("%d", &n);
			if (nVals != 1) {
				printf("Error: incorrect input format\n");
				badInput();
			}
			else if (n < 1) {
				printf("Error: the number of trapezoids must be greater than 1\n");
				badInput();
			}
		} while (nVals != 1 || n < 1);

		//Print the integral
		ans = integrate(min, max, n);
		printf("Using %d trapezoids, integral between %lf and %lf is %lf\n", n, min, max, ans);

		/*Prompting for another integral calculation. Error checks if value besides
		Y/y or X/x is entered. Ans is the answer to the question as a variable. cont will determine
		to continue going or not depending on the y/n answer.*/
		do {
			error = 0;
			printf("Calculate another Integral (Y/N)?: ");
			badInput();
			check = scanf("%c", &Ans);
			if (check != 1) {
				printf("Error: Incorrectly formatted input\n");
				badInput();
			}
			switch (Ans) {
			case 'Y':
			case 'y':
				printf("We will do another calculation\n");
				cont = 1;
				break;
			case 'N':
			case 'n':
				printf("The program will end\n");
				cont = 0;
				break;
			default:
				printf("Answer must be yes or no\n");
				error = 1;
				break;
			}
		} while (error == 1);
	} while (cont == 1);
	return 0;
}