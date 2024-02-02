/*
    This defines implementations for dictionary-related functions.
*/

#include "dict.h"
#include "data.h"
#include "ll.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define KEYDATAJOIN "-->"
#define FAILTEXT "NOTFOUND"
/*
struct dict {
    struct dataMapping *mapping;
    struct ll *list;
};

struct searchResult {
    struct key *key;
    
    struct data **data;
    struct dataMapping *mapping;
};
*/
struct dict *newDict(char *header){
    struct dict *returnDict = (struct dict *) malloc(sizeof(struct dict));
    assert(returnDict);
    returnDict->mapping = getDataMapping(header);
    returnDict->list = newList();
    return returnDict;
}

void prependRow(struct dict *dict, char *row){
    dict->list = prependToList(dict->list, dict->mapping, row);
}

struct searchResult *queryDict(struct dict *dict, char *string){
    struct key *searchKey = readKey(string, dict->mapping);
    struct searchResult *result = (struct searchResult *) 
        malloc(sizeof(struct searchResult));
    assert(result);
    result->key = searchKey;
    result->data = NULL;
    result->mapping = dict->mapping;
    
    searchLL(dict->list, dict->mapping, searchKey, &(result->data));
    
    return result;
}

void writeSearchResult(struct searchResult *result, struct dict *dict, FILE *file){
    char *keyText;
    char *resultText;
    int i;
    /* Commented out code is fairly clean way output-wise to display results */
    // int j;
    int first = 1;
    
    keyText = getKeyString(dict->mapping, result->key);
    
    // fprintf(file, "%s", keyText);
    
    // int indentLength = strlen(keyText);
    
    i = 0;
    while((result->data)[i]){
        // if(! first){
        //     for(j = 0; j < indentLength; j++){
        //         fprintf(file, " ");
        //     }
        // }
        fprintf(file, "%s", keyText);
        resultText = getDataString(dict->mapping, (result->data)[i]);
        fprintf(file, " %s %s\n", KEYDATAJOIN, resultText);
        if(resultText){
            free(resultText);
        }
        i++;
        first = 0;
    }
    if(first){
        fprintf(file, "%s\n", FAILTEXT);
    }
    if(keyText){
        free(keyText);
    }
}

void freeDict(struct dict *dict){
    freeLL(dict->list, dict->mapping);
    freeDataMapping(&(dict->mapping));
    free(dict);
}

void freeSearchResult(struct searchResult *result){
    if(result){
        freeKey(&(result->key), result->mapping);
        if(result->data){
            free(result->data);
        }
        free(result);
    }
}