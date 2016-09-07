#include <stdio.h>
#include "list.h"
#include <string.h>


typedef struct passwordRecord {
    char* username;
    unsigned int UID;
} passwordRecord;

#define BUFFERSIZE 1023


/*
 * function to free the allocated memory
 */
void passwordRecord_free(void *recordToFree){
    passwordRecord *record = (passwordRecord*)recordToFree;
    free(record->username);
    free(record);
}


/*
 * Opens either a file or stdin for reading
 */
FILE* getInputStream(int argc, char *argv[]){
    if(argc ==1){
        return stdin;
    } else if (argc == 2) {
        FILE * inFile = fopen(argv[1], "r");
        if ( !inFile ){
            fprintf(stderr, "File could not be opended\n");
        } else {

            fseek(inFile, 0, SEEK_END);
            unsigned long len = (unsigned long)ftell(inFile);
            rewind(inFile);
            // six separators would be need at least to proceed and fail in the further checks
            if(len < 6) {
                fclose(inFile);
                exit(0);
            }
        };
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

void listToStdOut(list *outputList){
    list_position currentPosition = list_first(outputList);
    while(list_isEnd(outputList, currentPosition) == false){
        data currentData = list_inspect(outputList, currentPosition);
        printf("%d:%s\n", ((passwordRecord*)currentData)->UID, ((passwordRecord*)currentData)->username);
        currentPosition = list_next(outputList, currentPosition);
    }
    // output last element
    data currentData = list_inspect(outputList, currentPosition);
    printf("%d:%s\n", ((passwordRecord*)currentData)->UID, ((passwordRecord*)currentData)->username);
}


int checkIndata(char* row, list* passwdList) {
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
        //printf("%s:%d\n", username, UID);
        passwordRecord *insertionRecord = malloc(sizeof(passwordRecord) * 1);
        insertionRecord->username = username;
        insertionRecord->UID = UID;
        if(list_isEmpty(passwdList)){
            list_insert(passwdList, list_first(passwdList),(data)insertionRecord);
        } else {
            list_position current = list_first(passwdList);
            while(((passwordRecord*)list_inspect(passwdList, current))->UID > insertionRecord->UID){
             current = list_next(passwdList, current);
                // have to improve that one...
                if(current == list_previous(passwdList, list_first(passwdList))){
                    break;
                }
            }
            list_insert(passwdList, current, (data)insertionRecord);

        }


    }
    if (rowError == 0){
        return 1;
    }
    return 0;
}




int main (int argc, char *argv[]){

    list *passwdList = list_empty();
    list_setMemHandler(passwdList, passwordRecord_free);
    char buffer[BUFFERSIZE];
    FILE *inStream = getInputStream(argc, argv);
    int successfulRows;
    int processedRows;

    while(fgets(buffer, BUFFERSIZE, inStream)){
        processedRows++;
        //printf("%s", buffer);
        successfulRows = successfulRows + checkIndata(buffer, passwdList);
    }

    if (successfulRows > 0){
        listToStdOut(passwdList);
    }

    list_free(passwdList);

    fclose(inStream);




}
