/*
 * mpasswdsort.h
 *
 * Lorenz Gerber
 * Laboration 1, Course 'System-level Programming' HT16
 *
 * This file contains the main() for a program to read unix 'passwd' files,
 * validate their content, store username and UID in a list and, sort the
 * list content and output it to standard out.
 */

#include <stdio.h>
#include "list.h"
#include <string.h>


typedef struct passwordRecord {
    char* username;
    unsigned int UID;
} passwordRecord;

#define BUFFERSIZE 1023

/*
 * passwordRecordFree
 *
 * function to free the allocated memory
 * in a passwordRecord
 *
 * recordToFree: Pointer to a passwordRecord struct
 */
void passwordRecordFree(void *recordToFree);

/*
 * getInputStream
 *
 * helper function used to determine if a file or stdin
 * is used as input.
 *
 * argc: number of command line arguments vom main program
 * argv: command line arguments from main
 *
 * FILE: input stream, either from file or stdin
 *
 */
FILE* getInputStream(int argc, char *argv[]);

/*
 * checkForChar
 *
 * Checks if the current char is separator or not.
 *
 * current: pointer to a char to check for separator
 * char:    check for this char
 *
 * bool: returns true if separator is found on current
 */
bool checkForChar(char* current, char* separator);

/*
 * find Separator
 *
 * Takes a string pointer and returns the position of the next spearator char
 *
 * current:     pointer to string
 * separator:   pointer to separator char to check for
 *
 * pos:         returns either a pointer to the next separator or the end of
 * the string
 */
char* findSeparator(char* current, char*separator);

/*
 * substring
 *
 * returns a pointer to the copy of substring from start to end
 *
 * start:   pointer to start of string to extract
 * end:     pointer to end of string to extract
 *
 * return: pointer to malloc allocated, extracted string
 */
char* substring(char* start, char* end);

/*
 * checkNumberOfSeparators
 *
 * determines how many separators occur in the current string
 *
 * string:      pointer to string to search for separators
 * separator:   pointer to char to search for in string
 *
 * counter:     number of separators found
 */
int checkNumberOfSeparators(char* string, char*separator);


/*
 * listToStdOut
 *
 * function to traverse a list and output it's content to stdout
 *
 * outputList:  list to be traversed and it's content written to stdout
 *
 */
void listToStdOut(list *outputList);


/*
 * checkIndata
 *
 * function that takes a string as input and checks whether it is according
 * to the set up rules for a UNIX passwd file
 *
 * row:         string to be checked
 * passwdList:  list to which the input shall be inserted
 * rowNumber:   used for output to stderr
 *
 * return:      returns 1 if the row was processed without error
 */
int checkIndata(char* row, list* passwdList, int rowNumber);
