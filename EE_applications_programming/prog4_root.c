/*Eric Swanson
Written : 2 / 23 / 17
The goal of this program is to use an iterative algorithm—an algorithm that runs until a given
condition is met—to approximate the nth root of a given value. The user will input two numbers,
a positive real number, A, and an integer,n. The nth root will be taken of the number A. While
loops, for loops, and if statements will be used for error checking and approximating the root.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	/*Input validation. A is a real number and the number that the root will be taken of. 
	n is an integer will be sued for the nth root of A. Checks with scanf() if user inputs 
	correct variable type, if A is positive, and if n is greater than 2.*/
	double A;
	int n;
	int nVals;
	char junk;
	int cont;
	do{
		do {
			printf("Enter a postive real number and an integer: ");
			nVals = scanf("%lf %d", &A, &n);
			if (nVals != 2) {
				printf("Incorrectly formatted input\n");
				do {
					scanf("%c", &junk);
				} while (junk != '\n');
			 }
			else if (A < 0) {
				printf("A must be a real number greater than 0\n");
				do {
					scanf("%c", &junk);
				} while (junk != '\n');
			}
			else if (n < 2){
				printf("n must be integer greater than 2\n");
				do {
					scanf("%c", &junk);
				} while (junk != '\n');
			}
		} while (nVals != 2 || A < 0 || n < 2);

		/*calculate the nth root. x0 is the result from the previous iteration
		of the calculation. xk1 is the result from the current calculation.*/
		int i;
		double x0 = 1;
		double xk1=1;
		double power;
		double abs;
	
			do {
				power = 1;
				for (i = n - 1; i > 0;i--) {
					power = power * x0;
				}
				xk1 = (1.0 / n)*((n - 1.0)*x0 + (A / power));
				abs = x0 - xk1;
				x0 = xk1;
				if (abs < 0) {
					abs *= -1;
				}
			} while (abs > 0.0000001);
			printf("Given A=%.2lf and n=%d, root = %.2lf \n", A, n, x0);

			/*Prompting for another root calculation. Error checks if value besides
			Y/y or X/x is entered. Ans is the answer to the question as a variable. cont will determine
			to continue going or not depending on the y/n answer.*/
			int error;
			char Ans;
			char check;
			do {
				error = 0;
				printf("Caculate another root (Y/N)?: ");
				do {
					scanf("%c", &junk);
				} while (junk != '\n');
				check = scanf("%c", &Ans);
				if (check != 1) {
					printf("Incorrectly formatted input\n");
					do {
						scanf("%c", &junk);
					} while (junk != '\n');
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
			} while ( error == 1);
	} while (cont == 1);
	return 0;
}