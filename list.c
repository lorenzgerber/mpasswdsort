#include <stdio.h>
#include "list.h"


//create empty list, constructor
list *list_empty(void) {
    list *newList=malloc(sizeof(struct list));
    newList->head=malloc(sizeof(element)); //huvudet för listan
    newList->head->next=newList->head;
    newList->head->previous=newList->head;
    newList->freeFunc=NULL;
    return newList;
}


// set mem handler
void list_setMemHandler(list *l, memFreeFunc *f){
    l->freeFunc=f;
}


// return first position
list_position list_first(list *l){
    return l->head->next;
}

// return last position
list_position list_last(list *l){
    return l->head->previous;
}

// get next position
list_position list_next(list *l, list_position p) {
    return p->next;
};

// get previous position
list_position list_previous(list *l, list_position p){
    return p->previous;
};

// is list empty
bool list_isEmpty(list *l) {
    return (l->head->next == l->head);
}

// insert list element
list_position list_insert(list *l,list_position p,data d) {
    list_position newPosition=malloc(sizeof(element));
    newPosition->data=d;
    newPosition->next=p->next;
    (p->next)->previous=newPosition;
    newPosition->previous=p;
    p->next=newPosition;
    return newPosition;

};

// inspect list element
data* list_inspect(list *l, list_position p) {
  return p->data;
};

// remove list element
list_position list_remove(list *l, list_position p) {
    list_position beforeRemove=p->previous;
    p->previous->next=p->next;
    p->next->previous=p->previous;
    if(l->freeFunc!=NULL)
        l->freeFunc(p->data);
    free(p);
    return beforeRemove;
};

// free memory
void list_free(list *l){
    list_position current = list_last(l);
    while (list_isEmpty(l) != true){
        current = list_remove(l, current);
    }
    free(l->head);
    free(l);
};



// check if end of list
bool list_isEnd(list *l, list_position p){
  if ( p->next == l->head) {
    return true;
  } else {
    return false;
  }
};
