/*Eric Swanson
First written : 3/20/17
The goal of this program is to create a histogram using arrays. The program reads text from the
keyboard and uses an array to track the number of times that each letter is entered. The histogram
will display the relative frequenciesof each letter. The user can use four main commands to run 
through this program: one to read a line of text, one to clear the letter counts to 0, one to print
the histogram, and one to quit the program.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include "prog6_functions.h"

int main() {
	char cmd; //value for command entered
	int cont; //value to determine if function will continue or not
	int histo[26] = { 0 }; //array to hold the frequency of letters entered
	int max = 0; //determines the ceiling for the histogram
	int i; //to track length of histo in for loop

	do {
		printf("Enter a command (C, R, P, or Q): ");
		scanf("%c", &cmd);
		cont = 0;
		switch (cmd) {
		case 'C': case 'c':
			//clears letter count to 0
			for (i = 0; i < 26;i++) {
				histo[i] = 0;
			}
			max = 1;
			cont = 1;
			badInput();
			printf("\n");
			break;
		case 'R': case 'r':
			//reads line of text and update letter counts in array
			ReadText(histo, &max);
			cont = 1;
			break;
		case 'P': case 'p':
			//prints histogram
			DrawHist(histo, max);
			cont = 1;
			badInput();
			break;
		case 'Q': case 'q':
			return 0;
			break;
		default:
			printf("Error: incorrect input\n\n");
			badInput();
			cont = 1;
		}
	} while (cont == 1);
}
