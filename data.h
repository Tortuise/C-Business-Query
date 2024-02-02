/*
    This defines prototypes for data-related functions.
*/
#ifndef DATAH
#define DATAH

struct data {
    char **data;
};

struct key {
    char **keys;
};

struct dataMapping {
    char **headers;
    int headerCount;
    /* Maps each key field to its index in the row. */
    int *keyLocations;
    /* Maps each data field to its index in the row. */
    int *dataLocations;
    int keyCount;
};

/*
    Read the given line and store the mapping from row to values.
*/
struct dataMapping *getDataMapping(char *header);

/*
    Read a line into the given key and data locations.
*/
void getData(char *row, struct dataMapping *mapping, struct key **key, 
    struct data **data);

/*
    Read a line as a key and return it.
*/
struct key *readKey(char *row, struct dataMapping *mapping);

/*
    Compare the two given keys. Returns -1 if the first key is less than the second,
    0 if they are a match and 1 if the second key is greater than the second.
*/
int compareKeys(struct key *firstKey, struct key *secondKey, struct dataMapping *mapping);
    
/*
    Frees the key and data in the given pair.
*/
void freeKeyPair(struct data **dataLoc, struct key **keyLoc, struct dataMapping *mapping);

/*
    Frees a standalone key.
*/
void freeKey(struct key **keyLoc, struct dataMapping *mapping);

/*
    Frees the given data mapping.
*/
void freeDataMapping(struct dataMapping **mapping);

/*
    Allocates space for a string representing the given
    key and returns it.
*/
char *getKeyString(struct dataMapping *mapping, struct key *key);

/*
    Allocates space for a string representing the given
    data and returns it.
*/
char *getDataString(struct dataMapping *mapping, struct data *data);
/*
	turn query key into array format
*/
char **getQueryKey(char *lineptr, char **query);
//free data in linked list
void freeListData(struct data *data, struct key *key);
#endif