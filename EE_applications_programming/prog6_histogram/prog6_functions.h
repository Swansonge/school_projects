/**********************************************
Eric Swnanson
Header file for histogram assignment
***********************************************/

// Lines used for conditional compilation--ensures header isn't included multiple times
#ifndef prog6_functions_h
#define prog6_functions_h

/* read one line of text from the keyboard, and update the array with the
number of occurences for each letter*/
void ReadText(int histo[], int *max);

/* Given the array holding the letter counts, draw the histogram with the 
letter frequencies*/
void DrawHist(int histo[], int max);

// Used to clear line if input formatting error occurs
void badInput();

//checks values in histo
void printArray(int histo[]);

#endif /* More conditional compilation--end of prog6_functions_h */