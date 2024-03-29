/*
    This defines prototypes for data-related functions.
*/
/* The number of indices expected in a row. */
#define DEFAULTINDICESCOUNT 1
#define NOTYETFOUND (-1)
#define JOINSTRING ", "
#define DATAJOINSTRING " || "
#define DATAMIDDLESTRING ": "

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


/* 
    The reference keys and data used for the problem. More flexible would be to
    get this from a file and store it in the struct. But at this point we don't 
    have that.
*/
static char *referenceKeys[] = {
    "x coordinate",
	"y coordinate"
};

static char *referenceData[] = {
    "Census year", 
    "Block ID", 
    "Property ID", 
    "Base property ID", 
    "CLUE small area", 
	"Trading name",
    "Industry (ANZSIC4) code", 
    "Industry (ANZSIC4) description", 
    "Location"
};

/*
    Find each comma separated value in the line and put the indices of when
    each value starts in location pointed to, store the number of comma separated
    values found in the indexCount.
*/
void splitTokens(char *line, int **indices, int *indexCount);

/*
    Get the string values from each token starting index, removing the quotes.
*/
char **extractTokens(char *line, int *indices, int indexCount);

void splitTokens(char *line, int **indices, int *indexCount){
    int inQuotes = 0;
    int progress = 0;
    int length = strlen(line);
    int allocatedIndices;
    *indexCount = 0;
    
    *indices = (int *) malloc(sizeof(int) * DEFAULTINDICESCOUNT);
    assert(*indices);
    allocatedIndices = DEFAULTINDICESCOUNT;
    
    /* First csv value always starts at index 0. */
    (*indices)[0] = 0;
    *indexCount = (*indexCount) + 1; 
    
    for(progress = 0; progress < length; progress++){
        if(! inQuotes){
            if(line[progress] == '\"'){
                inQuotes = 1;
            } else if(line[progress] == ','){
                /* Split token */
                if(allocatedIndices <= (*indexCount)){
                    *indices = (int *) realloc(*indices, 
                        sizeof(int) * allocatedIndices * 2);
                    assert(*indices);
                    allocatedIndices = allocatedIndices * 2;
                }
                (*indices)[*indexCount] = progress + 1;
                *indexCount = (*indexCount) + 1; 
            }
            /* Otherwise just move on. */
        } else {
            if(line[progress] == '\"'){
                inQuotes = 0;
            }
            /* Otherwise just move on. */
        }
    }
    
    /* Resize down indices */
    if(allocatedIndices > (*indexCount)){
        *indices = (int *) realloc(*indices, sizeof(int) * (*indexCount));
        assert(*indices);
    }
}

char **extractTokens(char *line, int *indices, int indexCount){
    /* Check lines with quotes and copy them to new allocations. */
    char **tokens = (char **) malloc(sizeof(char *) * indexCount);
    assert(tokens);
    int i, j, k;
    int start;
    int end;
    int quotePairs;
    for(i = 0; i < indexCount; i++){
        start = indices[i];
        if(i < (indexCount - 1)){
            end = indices[i + 1] - 2;
        } else {
            end = strlen(line) - 1;
        }
        if(line[start] == '\"' && line[end] == '\"'){
            start++;
            end--;
        }
        /* Work out how many pairs of quotes are present in the string. */
        quotePairs = 0;
        for(j = 0; j < (end - start + 1); j++){
            if(line[start + j] == '\"'){
                quotePairs++;
                /* Move ahead one character to skip second quote. */
                j++;
            }
        }
        
        tokens[i] = (char *) malloc(sizeof(char) * (end - start + 1 + 1 - quotePairs));
        assert(tokens[i]);
        k = 0;
        for(j = 0; j < (end - start + 1); j++){
            tokens[i][k] = line[start + j];
            if(line[start + j] == '\"'){
                /* Next character will be a double quote */
                assert(line[start + j + 1] == '\"');
                j++;
            }
            k++;
        }
        tokens[i][k] = '\0';
    }
    
    return tokens;
}

struct dataMapping *getDataMapping(char *header){
    int i, j;
    /* Split by comma. */
    int *indices = NULL;
    int indexCount = 0;
    
    splitTokens(header, &indices, &indexCount);
    
    char **tokens = extractTokens(header, indices, indexCount);
    
    /* Build map */
    struct dataMapping *returnMapping = (struct dataMapping *) 
        malloc(sizeof(struct dataMapping));
    assert(returnMapping);
    
    returnMapping->headers = tokens;
    returnMapping->headerCount = indexCount;
    returnMapping->keyCount = (int) (sizeof(referenceKeys) / sizeof(referenceKeys[0]));
    returnMapping->keyLocations = (int *) malloc(sizeof(int) * returnMapping->keyCount);
    assert(returnMapping->keyLocations);
    returnMapping->dataLocations = (int *) malloc(sizeof(int) * 
        (indexCount - returnMapping->keyCount));
    assert(returnMapping->dataLocations);
    
    /* Work out which are keys and which are data. */
    for(i = 0; i < returnMapping->keyCount; i++){
        /* Mark not yet found. */
        (returnMapping->keyLocations)[i] = NOTYETFOUND;
    }
    for(i = 0; i < (indexCount - returnMapping->keyCount); i++){
        /* Mark not yet found. */
        (returnMapping->dataLocations)[i] = NOTYETFOUND;
    }
    
    for(i = 0; i < indexCount; i++){
        /* Assume a small number of keys. */
        for(j = 0; j < returnMapping->keyCount; j++){
            if(strcmp(tokens[i], referenceKeys[j]) == 0){
                assert((returnMapping->keyLocations)[j] == NOTYETFOUND);
                (returnMapping->keyLocations)[j] = i;
                break;
            }
        }
        for(j = 0; j < (indexCount - returnMapping->keyCount); j++){
            if(strcmp(tokens[i], referenceData[j]) == 0){
                assert((returnMapping->dataLocations)[j] == NOTYETFOUND);
                (returnMapping->dataLocations)[j] = i;
                break;
            }
        }
    }
    
    /* We should have got all the values from the header. */
    for(j = 0; j < returnMapping->keyCount; j++){
        assert((returnMapping->keyLocations)[j] != NOTYETFOUND);
    }
    
    for(j = 0; j < (indexCount - returnMapping->keyCount); j++){
        assert((returnMapping->dataLocations)[j] != NOTYETFOUND);
    }
    
    if(indices){
        free(indices);
    }
    
    return returnMapping;
}

void getData(char *row, struct dataMapping *mapping, struct key **key, 
    struct data **data){
    int i;
    /* Split by comma. */
    int *indices = NULL;
    int indexCount = 0;
    
    splitTokens(row, &indices, &indexCount);
    
    char **tokens = extractTokens(row, indices, indexCount);
    
    assert(indexCount == mapping->headerCount);
    
    struct data *newData = (struct data *) malloc(sizeof(struct data));
    assert(newData);
    struct key *newKey = (struct key *) malloc(sizeof(struct key));
    assert(newKey);
    
    int dataCount = mapping->headerCount - mapping->keyCount;
    
    newData->data = (char **) malloc(sizeof(char *) * dataCount);
    assert(newData->data);
    newKey->keys = (char **) malloc(sizeof(char *) * mapping->keyCount);
    assert(newKey->keys);
    
    for(i = 0; i < dataCount; i++){
        (newData->data)[i] = tokens[(mapping->dataLocations)[i]];
    }
    for(i = 0; i < mapping->keyCount; i++){
        (newKey->keys)[i] = tokens[(mapping->keyLocations)[i]];
    }
    
    *key = newKey;
    *data = newData;
    if(tokens){
        free(tokens);
    }
    if(indices){
        free(indices);
    }
}

struct key *readKey(char *row, struct dataMapping *mapping){
    struct key *returnKey = (struct key *) malloc(sizeof(struct key));
    assert(returnKey);
    returnKey->keys = (char **) malloc(sizeof(char *) * mapping->keyCount);
    assert(returnKey->keys);
    
    (returnKey->keys)[0] = strdup(row);
    
    return returnKey;
}

int compareKeys(struct key *firstKey, struct key *secondKey, struct dataMapping *mapping){
    int i;
    int res;
    for(i = 0; i < mapping->keyCount; i++){
        /* Search is case sensitive */
        res = strcmp((firstKey->keys)[i], (secondKey->keys)[i]);
        // res = strcmp((firstKey->keys)[i], (secondKey->keys)[i]);
        if (res != 0){
            return res;
        }
    }
    /* All strcmp calls returned 0. */
    return 0;
}

void freeKeyPair(struct data **dataLoc, struct key **keyLoc, struct dataMapping *mapping){
    freeKey(keyLoc, mapping);
    int i;
    if(*dataLoc){
        if((*dataLoc)->data){
            for(i = 0; i < (mapping->headerCount - mapping->keyCount); i++){
                free(((*dataLoc)->data)[i]);
            }
            free((*dataLoc)->data);
        }
        free(*dataLoc);
    }
    *dataLoc = NULL;
}

void freeKey(struct key **keyLoc, struct dataMapping *mapping){
    int i;
    if(*keyLoc){
        if((*keyLoc)->keys){
            for(i = 0; i < mapping->keyCount; i++){
                free(((*keyLoc)->keys)[i]);
            }
            free((*keyLoc)->keys);
        }
        free(*keyLoc);
    }
    *keyLoc = NULL;
}

void freeDataMapping(struct dataMapping **mapping){
    if (! *mapping){
        return;
    }
    int i;
    if ((*mapping)->headers){
        for(i = 0; i < (*mapping)->headerCount; i++){
            free(((*mapping)->headers)[i]);
        }
        free((*mapping)->headers);
    }
    if((*mapping)->keyLocations){
        free((*mapping)->keyLocations);
    }
    if((*mapping)->dataLocations){
        free((*mapping)->dataLocations);
    }
    free(*mapping);
    *mapping = NULL;
}

char *getKeyString(struct dataMapping *mapping, struct key *key){
    int length = 0;
    int i, j, k;
    char *keyString;
    char *joinString = JOINSTRING;
    for(i = 0; i < mapping->keyCount; i++){
        length += strlen((key->keys)[i]);
        if((i + 1) < mapping->keyCount){
            length += strlen(joinString);
        }
    }
    
    keyString = (char *) malloc(sizeof(char) * (length + 1));
    assert(keyString);
    k = 0;
    for(i = 0; i < mapping->keyCount; i++){
        for(j = 0; j < strlen((key->keys)[i]); j++){
            keyString[k] = ((key->keys)[i])[j];
            k++;
        }
        if((i + 1) < mapping->keyCount){
            for(j = 0; j < strlen(joinString); j++){
                keyString[k] = joinString[j];
                k++;
            }
        }
    }
    keyString[k] = '\0';
    
    return keyString;
}

char *getDataString(struct dataMapping *mapping, struct data *data){
    int length = 0;
    int i, j, k;
    char *dataString;
    char *middleString = DATAMIDDLESTRING;
    char *joinString = DATAJOINSTRING;
    for(i = 0; i < ( mapping->headerCount - mapping->keyCount ); i++){
        length += strlen(referenceData[i]);
        length += strlen(middleString);
        length += strlen((data->data)[i]);
        length += strlen(joinString);
    }
    
    dataString = (char *) malloc(sizeof(char) * (length + 1));
    assert(dataString);
    k = 0;
    for(i = 0; i < (mapping->headerCount - mapping->keyCount); i++){
        for(j = 0; j < strlen(referenceData[i]); j++){
            dataString[k] = referenceData[i][j];
            k++;
        }
        for(j = 0; j < strlen(middleString); j++){
            dataString[k] = middleString[j];
            k++;
        }
        for(j = 0; j < strlen((data->data)[i]); j++){
            dataString[k] = ((data->data)[i])[j];
            k++;
        }
        for(j = 0; j < strlen(joinString); j++){
            dataString[k] = joinString[j];
            k++;
        }
    }
    dataString[k] = '\0';
    
    return dataString;
}
// get query keys from line
char **getQueryKey(char *lineptr, char **query) {
	
	query = malloc(strlen(lineptr) * sizeof(char));
	assert(query);
	//strtok break into array coordinates = [x,y]
	char *token;
	int i = 0;
	token = strtok(lineptr," ");
	while(token != NULL) {
		query[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	return query;
}
// free data in linked list
void freeListData(struct data *data, struct key *key) {
	if (data) {
		int i = 0;
		while(data->data[i]) {
			free(data->data[i]);
			i++;
		}
		free(data->data);
	}
	free(data);
	if (key) {
		int j = 0;
		while(key->keys[j]) {
			free(key->keys[j]);
			j++;
		}
		free(key->keys);
	}
	free(key);
}