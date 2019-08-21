#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "prog6_functions.h"

// Used to clear line if input formatting error occurs
void badInput() {
	char junk;

	do {
		scanf("%c", &junk);
	} while (junk != '\n');
}

/* read one line of text from the keyboard, and update the array with the
number of occurences for each letter*/
void ReadText(int histo[], int *max) {
	int ch; //ASCII value of characters in histo 
	char read='x'; //reads value of text entered
	int i; //for loop variable
	printf("\nENTER A LINE OF TEXT: \n\n");
	badInput();
	while (read != '\n' ) {
		scanf("%c", &read);
		ch = isalpha(read);
		if (ch != 0) {
			read=toupper(read);
			histo[read - 65] +=1;
		}
	}
	//printArray(histo);
	for (i = 0;i < 26;i++) {
		if (histo[i] > *max) {
			*max=histo[i];
		}
	}
	printf("\n");
}

/* Given the array holding the letter counts, draw the histogram with the
letter frequencies*/
void DrawHist(int histo[], int max) {
	int i; //for loop variable

	printf("\nLETTER FREQUENCIES IN TEXT:\n\n");
	for (;max > 0; max--) {
		for (i = 0; i < 26; i++) {
			if (histo[i] >= max) {
				printf("| ");
			}
			else printf("  ");
		}
		printf("\n");
	}
	max = 0;
	for (i = 0; i < 26; i++) {
		printf("+-");
	}
	printf("\nA B C D E F G H I J K L M N O P Q R S T U V W X Y Z\n\n");
}

//checks values in histo
void printArray(int histo[]) {
	int i;
	for (i = 0; i < 26; i++) {
		printf("%d ", histo[i]);
	}
	printf("\n");
}