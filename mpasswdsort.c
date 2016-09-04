#include <stdio.h>
#include "list.h"
#include <string.h>

typedef struct person {
  char firstname[12];
  char lastname[12];
} person;

#define SIZEOFFIRSTNAME 12
#define SIZEOFLASTNAME 12
#define BUFFERSIZE 1023

int main (int argc, char *argv[]){

    //Check if there is a command line argument


    //read data from file


    //Read Data from stdin
    // preliminary code to read from stdin
    if(argc==1){
        char buffer[BUFFERSIZE];
        while(fgets(buffer, BUFFERSIZE , stdin) != NULL)
        {
            printf("%s", buffer);
        }

    }

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

    data out = list_inspect(test, first);
    printf("%s \n", out);


}
