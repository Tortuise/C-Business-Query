/*
    This defines prototypes for linked-list-related functions.
*/
#ifndef LLH
#define LLH
#include "data.h"

struct ll {
    struct key *key;
    struct data *data;
    struct ll *next;
};

/* 
    Creates an empty linked list.
*/
struct ll *newList();

/*
    Prepend the item in the line to the given linked list.
*/
struct ll *prependToList(struct ll *ll, struct dataMapping *mapping, char *row);

/*
    Search for the key in the given list and put the data in the given location.
*/
void searchLL(struct ll *ll, struct dataMapping *mapping, struct key *key, 
    struct data ***data);

/*
    Frees all the memory in the given linked list.
*/
void freeLL(struct ll *ll, struct dataMapping *mapping);

/*
	copy data from list into another list and returns new list
*/
struct ll *newCopy(struct ll *list);

struct ll *addList(struct ll *list1, struct ll *list2);
void freeNodeList(struct ll *list);
#endif