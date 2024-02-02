#ifndef KDTREEH
#define KDTREEH
#define DIMENSIONS 2
#define TRUE 1
#define FALSE -1
#define NOTFOUND "NOTFOUND"

#include "ll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

struct treeNode {
    struct ll *list;
    struct treeNode *left;
    struct treeNode *right;

};
// create new node copying data from linked list
struct treeNode* newTreeNode(struct ll *list);
// insert record into a treenode in k-d tree
struct treeNode *insertRec(struct treeNode *root, struct ll *list, int depth);
// works recursvly
struct treeNode* insert(struct treeNode *root, struct ll *list);
// search k-d tree for nodes within radius
struct ll *searchRec(struct treeNode* root, struct ll *inRadius, int depth, char **query, int *numCmp, int *found, FILE *file);
// compute distance between points in node
double distance(double keyd1, double keyd2, double keyd3, double keyd4);
// write result to file
void writeKdTreeResult(char **query, struct ll *result, FILE *file, int *found);
// free tree
void freeTree(struct treeNode *node);
#endif