#include <stdio.h>
#include "list.h"

typedef struct *person {
  char firstname[12];
  char lastname[12];
} person;


int main (){
    printf("This is the shit\n");
    list *test = list_empty();
    if (list_isEmpty(test)){
      printf("Yes the list is empty\n");
    } else {
      printf("No the list is not empty\n");
    }




    person lorenz = {"Lorenz", "Gerber"};

}
