/*
    A search program find closest business from query point based on the CLUE database
    Run with
        dict datafile outputfile < inputfile
    
    where 
        datafile is a CLUE csv,
        outputfile is the name of the file which data will be output to,
    
    and
        inputfile is in the format of the business name to search for,
        with one business name per line.
    
    Written by Grady Fitzpatrick for Algorithms and Data Structures 2020S2
	
	Remodelled by Kenny Ung for assignment 2 1127378
	
	valgrind --leak-check=full --track-origins=yes ./map2 random_small.csv output.txt < input.txt
	big boi dataset ./map2 CLUEdata2018_random.csv output.txt < input.txt
	./map2 CLUEdata2018_median.csv output.txt < input.txt
	./map2 CLUEdata2018_sortx.csv output.txt < input.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "read.h"
#include "dict.h"
#include "utils.h"
#include "k-d_tree.h"
#include "ll.h"


int main(int argc, char **argv) {
    if(argc < 3) {
        fprintf(stderr, "Incorrect arguments\n");
        fprintf(stderr, "Run in the form\n");
        fprintf(stderr, "\tdict datafile outputfile < queryfile\n");
        exit(1);
    }
    
    char *line = NULL;
    size_t size = 0;
    
    struct dict *dict = readFile(argv[1]);
    //struct searchResult *result = NULL;
    
    FILE *outputFile = fopen(argv[2], "w");
    assert(outputFile);
    
    struct treeNode *root = NULL;
	
	struct ll *current = dict->list;
	// go through ll
    while (current != NULL) {
		// insert every record from ll into k-d tree
        root = insertRec(root, current,0);
        current = current->next;
    }
	current = dict->list;
	// get line from input text
    while(getlineClean(&line, &size, stdin) != (-1)) {
		char **query;
		
		int numCmp = 0;
		int found = FALSE;
		struct treeNode *nearestNode = (struct treeNode *)malloc(sizeof(struct treeNode));
		struct ll *inRadius = (struct ll *)malloc(sizeof(struct ll));
		assert(nearestNode);
		assert(inRadius);
		
		
		query = getQueryKey(line, query);
		
		// search for any nodes within radius and write to outputfile
		inRadius = searchRec(root, inRadius, 0, query, &numCmp, &found, outputFile);
		if (found == FALSE) {
		writeKdTreeResult(query, inRadius, outputFile, &found);
		}
		printf("%s %s %s --> %d \n",query[0],query[1],query[2],numCmp);
		
		free(nearestNode);
		free(inRadius);
		free(query);
    }
	
	// free k-d tree (fix)
	//if(root) {
	//	freeTree(root);
	//}
    if(line) {
        free(line);
    }
    if(dict) {
        freeDict(dict);
    }
    if(outputFile) {
        fclose(outputFile);
    }
    
    return 0;
}