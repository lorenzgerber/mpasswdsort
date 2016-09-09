/*
 * mpasswdsort.c
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
void passwordRecordFree(void *recordToFree){
    passwordRecord *record = (passwordRecord*)recordToFree;
    free(record->username);
    free(record);
}

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
FILE* getInputStream(int argc, char *argv[]){
    if(argc ==1){
        return stdin;
    } else if (argc == 2) {
        FILE * inFile = fopen(argv[1], "r");
        if ( !inFile ){
            perror(argv[1]);
            exit(EXIT_FAILURE);
        } else {

            fseek(inFile, 0, SEEK_END);
            unsigned long len = (unsigned long)ftell(inFile);
            rewind(inFile);
            // six separators would be need at least to
            // proceed and fail in the further checks
            if(len < 6) {
                if(fclose(inFile) != 0)
                    perror(argv[1]);
                exit(0);
            }
        };
        return inFile;
    }
    return stdin;
}

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
bool checkForChar(char* current, char* separator){
    char *found = strstr(current, separator);
    if(found == current){
        return true;
    }
    return false;
}

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
char* findSeparator(char* current, char*separator){
    char* pos = strstr(current, separator);
    if (pos == NULL){
        pos = current + strlen(current) - 1;
    }
    return pos;
}

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
char* substring(char* start, char* end){
    int length = 0;
    while (end > start){
        end -= 1;
        length++;
    }

    char * extracted = strndup(start, length);
    if (extracted == NULL){
        perror(start);
    }
    return extracted;
}

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
int checkNumberOfSeparators(char* string, char*separator){
    int counter = 0;
    char*current = string;
    while(findSeparator(current, separator) != (string + strlen(string) - 1)){
        counter++;
        current = findSeparator(current, separator) + 1;
    }
    return counter;
}


/*
 * listToStdOut
 *
 * function to traverse a list and output it's content to stdout
 *
 * outputList:  list to be traversed and it's content written to stdout
 *
 */
void listToStdOut(list *outputList){
    listPosition currentPosition = listFirst(outputList);
    while(listIsEnd(outputList, currentPosition) == false){
        data currentData = listInspect(outputList, currentPosition);
        printf("%d:%s\n", (int)((passwordRecord*)currentData)->UID,
               (char*)((passwordRecord*)currentData)->username);
        currentPosition = listNext(outputList, currentPosition);
    }
    // output last element
    data currentData = listInspect(outputList, currentPosition);
    printf("%d:%s\n", ((passwordRecord*)currentData)->UID,
           ((passwordRecord*)currentData)->username);
}


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
int checkIndata(char* row, list* passwdList, int rowNumber) {
    char* separator = ":";
    char* result;
    char* username;
    int UID;
    int rowError = 0;
    int usernameAllocated = 0;


    // check for blank line
    if(strlen(row) == 1){
        fprintf(stderr, "Line %d: Encountered a <BLANKLINE>\n", rowNumber);
        rowError = -1;
    }

    // check if correct number of separators is there
    if(rowError == 0){
        if(checkNumberOfSeparators(row, separator) < 6){
            fprintf(stderr, "Line %d: Invalid format: %s\n", rowNumber, row);
            rowError = -1;
        }
    }

    // check if username exists
    if(rowError == 0){
        if(checkForChar(row, separator)){
            fprintf(stderr, "Line %d: The username field cannot be empty.\n",
                    rowNumber);
            rowError = -1;
        }
    }

    // Check username length
    char*start = row;
    char*end = findSeparator(start, separator);
    if (rowError == 0){
        result =  substring(start, end);
        if(strlen(result) > 32){
            fprintf(stderr, "Line %d: The username is too long, max 32 "
                            "chars\n", rowNumber);
            rowError = -1;
        } else {
            username = strdup(result);
            if (username == NULL){
                perror(result);
            }
            usernameAllocated = 1;
        }
        free(result);
    }


    // step through the password
    if (rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
    }


    if(rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        char *rest = NULL;
        long test = strtol(result, &rest, 10);
        if(test < 0){
            fprintf(stderr, "Line %d: UID has to be a positive number. "
                    "Got \"%s\"\n", rowNumber, result);
            rowError = -1;
        } else if ( *rest != 0) {
            fprintf(stderr, "Line %d: UID has to be a number. Got \"%s\"\n",
                    rowNumber, result);
            rowError = -1;
        } else {
            UID = test;
        }
        free(result);
    }


    if(rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result = substring(start, end);
        char *rest = NULL;
        long test = strtol(result, &rest, 10);
        if(test < 0){
            fprintf(stderr, "Line %d: GID has to be a positive number. Got "
                    "\"%s\"\n", rowNumber, result);
            rowError = -1;
        } else if( *rest != 0){
            fprintf(stderr, "Line %d: GID has to be a number. Got \"%s\"\n",
                    rowNumber, result);
            rowError = -1;
        }
        free(result);
    }

    // go through GECOS
    if (rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
    }

    // check if path is empty
    if (rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        if(strlen(result) == 0){
            fprintf(stderr, "Line %d: The path filed cannot be empty.\n",
                    rowNumber);
            rowError = -1;
        }
        free(result);
    }

    // check if shell is a password entry
    if (rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        if(strlen(result) == 0){
            fprintf(stderr, "Line %d: The shell field cannot be empty.\n",
                    rowNumber);
            rowError = -1;
        }
        free(result);
    }

    // insertion sort into list
    if(rowError == 0){
        int inserted = 0;
        passwordRecord *insertionRecord = malloc(sizeof(passwordRecord) * 1);
        if (insertionRecord == NULL){
            perror("Memory allocation of record to insert in list\n");
        }
        insertionRecord->username = username;
        insertionRecord->UID = UID;
        if(listIsEmpty(passwdList)){
            listInsert(passwdList, listFirst(passwdList),
                        (data)insertionRecord);
        } else {
            listPosition current = listPrevious(passwdList, listFirst
                                                          (passwdList));

            while(((passwordRecord*)listInspect(passwdList, listNext
                    (passwdList, current)))->UID < insertionRecord->UID) {
                current = listNext(passwdList, current);
                if (listIsEnd(passwdList, current)){
                    listInsert(passwdList, current, (data)insertionRecord);
                    inserted = 1;
                    break;
                }
            }
            if (inserted == 0){
                listInsert(passwdList, current, (data)insertionRecord);
            }
        }
    }

    if(usernameAllocated == 1 && rowError == -1){
        free(username);
    }


    if (rowError == 0){
        return 1;
    }
    return 0;
}



/*
 * mpasswdsort
 *
 * Program to validate, sort and output UNIX passwd files
 *
 * Usage: a passwd file can be entered as commandline argument.
 * Alternatively, the passwd file can also be fed in through stdin
 *
 */
int main (int argc, char *argv[]){


    char buffer[BUFFERSIZE];
    FILE *inStream = getInputStream(argc, argv);
    list *passwdList = listEmpty();
    listSetMemHandler(passwdList, passwordRecordFree);
    int successfulRows = 0;
    int processedRows = 0;

    /*
     * Looping through stream input rows
     */
    while(fgets(buffer, BUFFERSIZE, inStream)){
        processedRows++;
        successfulRows = successfulRows + checkIndata(buffer, passwdList,
                                                      processedRows);
    }

    /*
     * Data output to Stdout
     */
    if (successfulRows > 0){
        listToStdOut(passwdList);
    }


    /*
     * Clean up
     */
    listFree(passwdList);
    fclose(inStream);

    if (processedRows>successfulRows){
        exit(EXIT_FAILURE);
    }
     return 0;

}
