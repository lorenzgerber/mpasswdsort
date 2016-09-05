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
    char* password;
    int UID;
    int GID;
    int GECOS;
    char* path;
    char* shell;
} passwordRecord;

#define SIZEOFFIRSTNAME 12
#define SIZEOFLASTNAME 12
#define BUFFERSIZE 1023

FILE* getInputStream(int argc, char *argv[]){
    if(argc ==1){
        return stdin;
    } else if (argc == 2) {
        FILE * inFile = fopen(argv[1], "r");
        return inFile;
    }
    return stdin;
}

bool checkForChar(char* current, char* separator){
    char *found = strstr(current, separator);
    if(found == current){
        return true;
    }
    return false;
}

char* findSeparator(char* current, char*separator){
    char* pos = strstr(current, separator);
    if (pos == NULL){
        pos = current + strlen(current) - 1;
    }
    return pos;
}

char* substring(char* start, char* end){
    int length = 0;
    while (end > start){
        end -= 1;
        length++;
        //printf("%d\n", length);
    }

    return strndup(start, length);
}


void checkIndata(char* row) {
    char* separator = ":";
    char* result;

    if(checkForChar(row, separator))
        printf("the first is a semicolon\n");

    char*start = row;
    char*end = findSeparator(start, separator);
    result =  substring(start, end);
    printf("%s\n", result);
    free(result);

    start = end + 1;
    end = findSeparator(start, separator);
    result =  substring(start, end);
    printf("%s\n", result);
    free(result);

    start = end + 1;
    end = findSeparator(start, separator);
    result =  substring(start, end);
    printf("%s\n", result);
    free(result);

    start = end + 1;
    end = findSeparator(start, separator);
    result =  substring(start, end);
    printf("%s\n", result);
    free(result);

    start = end + 1;
    end = findSeparator(start, separator);
    result =  substring(start, end);
    printf("%s\n", result);
    free(result);

    start = end + 1;
    end = findSeparator(start, separator);
    result =  substring(start, end);
    printf("%s\n", result);
    free(result);

    start = end + 1;
    end = findSeparator(start, separator);
    result =  substring(start, end);
    printf("%s\n", result);
    free(result);

}

int checkNumberOfEntries(char* row) {
    return 1;
}

int main (int argc, char *argv[]){

    char buffer[BUFFERSIZE];
    FILE *inStream = getInputStream(argc, argv);
    while(fgets(buffer, BUFFERSIZE, inStream)){
        //printf("%s", buffer);
        checkIndata(buffer);
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



    list *test = list_empty();
    if (list_isEmpty(test)){
      printf("Yes the list is empty\n");
    } else {
      printf("No the list is not empty\n");
    }





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



}
