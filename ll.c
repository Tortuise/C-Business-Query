/*
    This defines implementations for linked-list-related functions.
*/

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct ll {
    struct key *key;
    struct data *data;
    struct ll *next;
};

/* Create a new empty linked list node. */
struct ll *newNode();

struct ll *newList(char *header){
    return NULL;
}

struct ll *newNode(){
    struct ll *returnNode = (struct ll *) malloc(sizeof(struct ll));
    assert(returnNode);
    returnNode->key = NULL;
    returnNode->data = NULL;
    returnNode->next = NULL;
    return returnNode;
}

struct ll *prependToList(struct ll *ll, struct dataMapping *mapping, char *row){
    struct ll *newHead = newNode();
    getData(row, mapping, &(newHead->key), &(newHead->data));
    newHead->next = ll;
    return newHead;
}

void searchLL(struct ll *ll, struct dataMapping *mapping, struct key *key, 
    struct data ***data){
    struct ll *current = ll;
    int foundMatches = 0;
    /* NULL terminated array. */
    *data = (struct data **) malloc(sizeof(struct data *));
    **data = NULL;
    while(current != NULL){
        while(current != NULL && compareKeys(current->key, key, mapping) != 0){
            current = current->next;
        }
        if(current && compareKeys(current->key, key, mapping) == 0){
            *data = realloc(*data, sizeof(struct data *) * (foundMatches + 2));
            assert(*data);
            (*data)[foundMatches] = current->data;
            (*data)[foundMatches + 1] = NULL;
            foundMatches++;
            /* We're done with this one, so move to next node. */
            current = current->next;
        }
    }
}

void freeLL(struct ll *ll, struct dataMapping *mapping){
    if(! ll){
        return;
    }
    struct ll *current = ll;
    struct ll *next;
    while(current){
        next = current->next;
        freeKeyPair(&(current->data), &(current->key), mapping);
        free(current);
        current = next;
    }
}
struct ll *newCopy(struct ll *list) {
	struct ll *newList = (struct ll *) malloc(sizeof(struct ll));
	assert(newList);
	struct data *newData = (struct data *) malloc(sizeof(struct data));
    assert(newData);
    struct key *newKey = (struct key *) malloc(sizeof(struct key));
    assert(newKey);
	
	newData = list->data;
	newKey = list->key;
	newList->data = newData;
	newList->key = newKey;
	
	return newList;
	
}
/*
  add nodes from one linked list into another
*/
struct ll *addList(struct ll *list1, struct ll *list2){
	//list 1 inradius
	// list 2 what want to add 
	//printf("test list2 %s \n",list2->key->keys[0]);
	while (list2 != NULL) {
		
		struct ll *newHead = (struct ll *)malloc(sizeof(struct ll));
		assert(newHead);
		
		newHead->next = list1;
		newHead = newCopy(list2);
		list1 = newHead;
		
		list2 = list2->next;
	}
	//printf("test addlist \n");
	
	//printf("test list1 %s \n",list1->key->keys[0]);
	return list1;
}
// free linked list in node
void freeNodeList(struct ll *list) {
	if(! list){
        return;
    }
    struct ll *current = list;
    struct ll *next;
    while(current){
        next = current->next;
		freeListData(current->data, current->key);
        free(current);
        current = next;
    }
}