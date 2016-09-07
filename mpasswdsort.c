#include <stdio.h>
#include "list.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct person {
  char firstname[12];
  char lastname[12];
} person;

typedef struct passwordRecord {
    char* username;
    unsigned int UID;
} passwordRecord;

#define SIZEOFFIRSTNAME 12
#define SIZEOFLASTNAME 12
#define BUFFERSIZE 1023

/*
 * Opens either a file or stdin for reading
 */
FILE* getInputStream(int argc, char *argv[]){
    if(argc ==1){
        return stdin;
    } else if (argc == 2) {
        FILE * inFile = fopen(argv[1], "r");
        return inFile;
    }
    return stdin;
}

/*
 * Checks if the current char is separator or not. *Obsolete/overkill*
 */
bool checkForChar(char* current, char* separator){
    char *found = strstr(current, separator);
    if(found == current){
        return true;
    }
    return false;
}

/*
 * returns the next position of the separator in current
 */
char* findSeparator(char* current, char*separator){
    char* pos = strstr(current, separator);
    if (pos == NULL){
        pos = current + strlen(current) - 1;
    }
    return pos;
}

/*
 * returns a pointer to the copy of substring from start to end
 */
char* substring(char* start, char* end){
    int length = 0;
    while (end > start){
        end -= 1;
        length++;
        //printf("%d\n", length);
    }

    return strndup(start, length);
}

int checkNumberOfSeparators(char* string, char*separator){
    int counter = 0;
    char*current = string;
    while(findSeparator(current, separator) != (string + strlen(string) - 1)){
        counter++;
        current = findSeparator(current, separator) + 1;
    }
    return counter;
}


void checkIndata(char* row, list* passwdList) {
    char* separator = ":";
    char* result;
    char* username;
    int UID;
    int rowError = 0;


    if(checkNumberOfSeparators(row, separator) < 6){
        fprintf(stderr, "wrong structure\n");
        rowError = -1;
    }

    if(rowError == 0){
        if(checkForChar(row, separator)){
            fprintf(stderr, "username is missing\n");
            rowError = -1;
        }
    }


    char*start = row;
    char*end = findSeparator(start, separator);

    // check for the user name
    if (rowError == 0){
        result =  substring(start, end);
        if(strlen(result) > 32){
            fprintf(stderr, "username too long\n");
            rowError = -1;
        } else {
            username = strdup(result);
        }
        //printf("%s\n", result);
        free(result);
    }


    // check if there is a password entry
    if (rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        if(strlen(result) == 0){
            fprintf(stderr, "password is empty\n");
            rowError = -1;
        }
        //printf("%s\n", result);
        free(result);
    }


    if(rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        long test = strtol(result, NULL, 10);
        if(test < 0){
            fprintf(stderr, "negativ UID value\n");
            rowError = -1;
        } else {
            UID = test;
        }
        //printf("%ld\n", test);
        free(result);
    }


    if(rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        long test = strtol(result, NULL, 10);
        if(test < 0){
            fprintf(stderr, "negativ GID value\n");
            rowError = -1;
        }
        //printf("%s\n", result);
        free(result);
    }


    // not needed when checked first for
    // correct number of separators
    if(rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        //printf("%s\n", result);
        free(result);
    }



    // check if path is empty
    if (rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        if(strlen(result) == 0){
            fprintf(stderr, "path is empty\n");
            rowError = -1;
        }
        //printf("%s\n", result);
        free(result);
    }

    // check if shell is a password entry
    if (rowError == 0){
        start = end + 1;
        end = findSeparator(start, separator);
        result =  substring(start, end);
        if(strlen(result) == 0){
            fprintf(stderr, "shell is empty\n");
            rowError = -1;
        }
        //printf("%s\n", result);
        free(result);
    }

    // insertion sort into list
    if(rowError == 0){
        printf("%s:%d\n", username, UID);
        passwordRecord *insertionRecord = malloc(sizeof(passwordRecord) * 1);
        insertionRecord->username = username;
        insertionRecord->UID = UID;
        if(list_isEmpty(passwdList)){
            list_insert(passwdList, list_first(passwdList),(data)insertionRecord);
        } else {
            list_position current = list_first(passwdList);
            while(((passwordRecord*)list_inspect(passwdList, current))->UID < insertionRecord->UID){
             current = list_next(passwdList, current);
                // have to improve that one...
                if(current == list_previous(passwdList, list_first(passwdList))){
                    break;
                }
            }
            list_insert(passwdList, current, (data)insertionRecord);

        }

    }


}

int checkNumberOfEntries(char* row) {
    return 1;
}

int main (int argc, char *argv[]){

    list *passwdList = list_empty();
    char buffer[BUFFERSIZE];
    FILE *inStream = getInputStream(argc, argv);
    while(fgets(buffer, BUFFERSIZE, inStream)){
        //printf("%s", buffer);
        checkIndata(buffer, passwdList);
    }

    fclose(inStream);



    // function that takes a whole line as input and validates the passwd format
    // username:password:UID:GID:GECOS:directory:shell
    // check username not empty and not longer than 32 chars
    // check UID not negative
    // check GID not negative
    // check directory not empty
    // check shell not empty

    // write data in list

    // sort list

    // travers list and output to std out




    if (list_isEmpty(passwdList)){
      printf("Yes the list is empty\n");
    } else {
      printf("No the list is not empty\n");
    }





    /*
    person* lorenz = malloc(sizeof(person) * 1);
    memset(lorenz->firstname,'\0',SIZEOFFIRSTNAME);
    strcpy(lorenz->firstname,"Lorenz");

    memset(lorenz->lastname, '\0', SIZEOFLASTNAME);
    strcpy(lorenz->lastname, "Gerber");

    person* fritz = malloc(sizeof(person) *1);
    memset(fritz->firstname,'\0',SIZEOFFIRSTNAME);
    strcpy(fritz->firstname,"Fritz");

    memset(fritz->lastname, '\0', SIZEOFLASTNAME);
    strcpy(fritz->lastname, "Meier");


    list_position first = list_insert(test, list_first(test), lorenz);
    list_position second = list_insert(test, first, fritz );

    list_remove(test, first);
    list_remove(test, second);
    //data out = list_inspect(test, first);
    //printf("%s \n", ((struct person*)out)->firstname);

    free(lorenz);
    free(fritz);

    list_free(test);
     */



}
