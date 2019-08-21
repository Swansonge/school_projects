/*Eric Swanson
Written: 2/9/17
The goal of this program is to implement a simple calculator using conditional statements.
Using printf(), a user is prompted to enter two numbers (operands) and an arithmetic operator, which
are then read by a scanf() function. Using conditional switch cases, the program will execute
the arithmetic operation. If the inputs are not valid, or an invalid operation is entered,
the program will say so and end. Error checking is done using conditional if statements.
If the inputs are valid, the operation and its result will be printed. */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	//asking user for precision and error checking. prec is the precision value. The pVal checks
	//if the input is an integer. If the input is valid, the program continues. If the input is
	//not an int, or is less than 0, an error message is displayed. 
	int prec;
	printf("Enter a precision: "); 
	int pVal = scanf("%d", &prec);
	if (pVal != 1 || prec < 0) {
		printf("Error: the value must be an INTEGER greater than or equal to 0\n");
		return 0;
	}


	//asking user for numbers and their operators. num1 and num2 are the two operand values.
	//op is the operator. nVal checks is three correct inputs were entered. numbers and an
	//operator are entered, the program continues.If not, an error message is displayed. 
	double num1, num2;
	char op;
	printf("Enter an arithemtic operation with two numbers: ");
	int nVal = scanf("%lf %c %lf", &num1, &op, &num2);
	if (nVal != 3) {
		printf("Error: input must be two numbers and operator\n");
		return 0;
	}

	//performing the operation. res is result of operation. If user tries to divide 
	//by zero, an error message is displayed. Otherwise, the operation is carried out 
	//and the result is displayed with the correct precision. The switch cases check the 
	//value of op. Depending on its value, it will execute the operation. If an invalid 
	//operator is entered, it will display and error message.
	double res;
	if (num2 == 0 && op == '/') {
		printf("Error: cannot divide by 0\n");
		return 0;
	}
	switch (op) {
	case '+':
		res = num1 + num2;
		break;
	case '-':
		res = num1 - num2;
		break;
	case '*':
		res = num1 * num2;
		break;
	case '/':
		res = num1 / num2;
		break;
	default:
		printf("Error: invalid operator entered\n");
		return 0;
	}

	//the result
	printf("%.*lf %c %.*lf = %.*lf\n", prec, num1, op, prec, num2, prec, res);
	return 0;
}