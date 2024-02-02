/*
    This defines prototypes for dictionary-related functions.
*/
#ifndef DICTH
#define DICTH
#include <stdio.h>

struct dict {
    struct dataMapping *mapping;
    struct ll *list;
};

struct searchResult {
    struct key *key;
    /* NULL terminated array of data. */
    struct data **data;
    struct dataMapping *mapping;
};
/*
    Returns a new dictionary.
*/
struct dict *newDict(char *header);

/*
    Add the given row to the dictionary.
*/
void prependRow(struct dict *dict, char *row);

/*
    Search the given key in the dictionary.
*/
struct searchResult *queryDict(struct dict *dict, char *string);

/*
    Print the given search result to a file.
*/
void writeSearchResult(struct searchResult *result, struct dict *dict, FILE *file);

/*
    Frees all memory associated with dictionary.
*/
void freeDict(struct dict *dict);

/*
    Frees anything used only by the search result.
*/
void freeSearchResult(struct searchResult *result);
#endif