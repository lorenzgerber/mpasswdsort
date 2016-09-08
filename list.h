/*
 * list.h
 *
 * Lorenz Gerber
 *
 * Laboration 1, Course 'System-level programming', HT16
 *
 * header file for implementation of a double linked list using
 * generic/void data fields.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef void *data;

typedef void memFreeFunc(data);

// element for a double linked list
typedef struct element {
    struct element *previous;
    data data;
    struct element *next;
} element;

typedef element * list_position;

// head for a double linked list
typedef struct list {
    element *head;
    memFreeFunc *freeFunc;
} list;



//create empty list, constructor
list *list_empty(void);


// set mem handler
void list_setMemHandler(list *l, memFreeFunc *f);


// return first position
list_position list_first(list *l);

// reutrn last position
list_position list_last(list *l);

// get next position
list_position list_next(list *l, list_position p);

// get previous position
list_position list_previous(list *l, list_position p);

// is list empty
bool list_isEmpty(list *l);

// insert list element
list_position list_insert(list *l,list_position p,data d);

// inspect list element
data* list_inspect(list *l, list_position p);

// remove list element
list_position list_remove(list *l, list_position p);

// free memory
void list_free(list *l);



// check if end of list
bool list_isEnd(list *l, list_position p);