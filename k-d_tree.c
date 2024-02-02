/**
 * Defines a k-d tree of records
 * 
 *  All functions relating to k-d trees
 * 
 * 
**/
#include "ll.h"
#include "k-d_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>


int k = DIMENSIONS;


// create new node copying data from linked list
struct treeNode* newTreeNode(struct ll *list) 
{ 
    struct treeNode* temp = malloc(sizeof(struct treeNode)); 

    assert(temp);
	temp->list = newCopy(list);//copy list
    temp->left = NULL;
    temp->right = NULL;
	//printf("new node made with key (%s %s) \n",temp->list->key->keys[0],temp->list->key->keys[1]);
    return temp; 
} 

struct treeNode *insertRec(struct treeNode *root, struct ll *list, int depth) 
{ 
    // Tree is empty? 
    if (root == NULL) 
       return newTreeNode(list); 
  
    // Calculate current dimension (cd) of comparison 
    int cd = depth % k; 
  
    // Compare the new point with root on current dimension 'cd' 
    // and decide the left or right subtree 
    // copy values into doubles
	char *key1 = list->key->keys[cd];
	char *key2 = root->list->key->keys[cd];
	//other dimension coords
	char *key3 = list->key->keys[1-cd];
	char *key4 = root->list->key->keys[1-cd];
	
	char *ptr;
	double keyd1 = strtod(key1, &ptr);
	double keyd2 = strtod(key2, &ptr);
	double keyd3 = strtod(key3, &ptr);
	double keyd4 = strtod(key4, &ptr);
	// if less then move left
    if (keyd1 < keyd2) {
        root->left  = insertRec(root->left, list, depth + 1); 
	
    }
	// if partial match or greater than insert right
    if ( (keyd1 >= keyd2) && (keyd3 != keyd4) ) {
        root->right = insertRec(root->right, list, depth + 1); 
    }
    // if same then add root list to same node
    else { 
		if (keyd1 == keyd2) {
			//make seperate linked list as root node then append 
			struct ll *new_list = newCopy(list);			
			new_list->next = root->list;
			root->list = new_list;		
		}
    }
    return root; 
} 
  
// Function to insert a new point with given point in 
// KD Tree and return new root. It mainly uses above recursive 
// function "insertRec()" 
struct treeNode* insert(struct treeNode *root, struct ll *list) 
{ 
    return insertRec(root, list, 0); 
} 

// compute distance between doubles in treeNode and query
double distance(double keyd1, double keyd2, double keyd3, double keyd4) 
{ 
	double dist;
	double point1 = (keyd2 - keyd1);
	double point2 = (keyd4 - keyd3);
    // Compare each keys x and y
	dist = (point1 * point1 + point2 * point2);
    return dist; 
} 

// search k-d tree for nodes within radius
struct ll *searchRec(struct treeNode *root, struct ll *inRadius, int depth, char **query, int *numCmp,int *found, FILE *file)
{ 
    // Base cases 
    if (root == NULL) {
        return NULL;
	}
    // Current dimension is computed using current depth and total 
    int cd = depth % k; 
	 // copy values into doubles
	char *rad = query[2];
	char *key1 = query[cd], *key2 = root->list->key->keys[cd];
	//other dimension coords
	char *key3 = query[1-cd], *key4 = root->list->key->keys[1-cd];	
	char *ptr;
	double radd = strtod(rad, &ptr);
	double keyd1 = strtod(key1, &ptr),keyd2 = strtod(key2, &ptr),keyd3 = strtod(key3, &ptr),keyd4 = strtod(key4, &ptr);		
	double dist = distance(keyd1, keyd2, keyd3, keyd4);	
	double sqrtdist = sqrt(dist);

	// if in radius add node to linked list
	if (sqrtdist < radd) {
		*found = TRUE;
		//inRadius = addList(inRadius, root->list);
		writeKdTreeResult(query,root->list,file, found);
	}
	(*numCmp)++;
	
	double dimensionDist = fabs(keyd1 - keyd2);
	dimensionDist *= dimensionDist;
    // Compare point with root with respect to cd (Current dimension) 
    if (keyd1 < keyd2 ) {
	
		searchRec(root->left, inRadius, depth + 1, query, numCmp, found, file); 
		
		if (dimensionDist < radd) {
			searchRec(root->right, inRadius, depth + 1, query, numCmp, found, file); 
		}
	//check min dist and try other side
	}
	if (keyd1 >= keyd2) {
		
		searchRec(root->right, inRadius, depth + 1, query, numCmp, found, file); 
		
		if (dimensionDist < radd) {
			searchRec(root->left, inRadius, depth + 1, query, numCmp, found, file); 
		}
	}
	//printf("returning %s\n",inRadius->data->data[5]);
	return inRadius;
}

// write result to outputfile
void writeKdTreeResult(char **query, struct ll *result, FILE *file, int *found) {
	
	struct ll *temp = result;
	// if not found
	if (*found == FALSE) {
		fprintf(file,"%s %s %s --> %s\n", query[0], query[1], query[2], NOTFOUND );
	}
	if (*found == TRUE) {
		while (temp != NULL) {
			fprintf(file, "%s %s %s --> Census year: %s || Block ID: %s || Property ID: %s || Base property ID: %s || CLUE small area: %s || Trading name: %s || Industry (ANZSIC4) code: %s || Industry (ANZSIC4) description: %s || x coordinate: %s || y coordinate: %s || Location: %s || \n", query[0], query[1], query[2],temp->data->data[0], temp->data->data[1], temp->data->data[2], temp->data->data[3], temp->data->data[4], temp->data->data[5] , temp->data->data[6], temp->data->data[7],temp->key->keys[0],temp->key->keys[1], temp->data->data[8]);
			temp = temp->next;
		}	
	}	
}
// free k-d tree
void freeTree(struct treeNode *node) {
	if (node == NULL) {
		return;
	}
	freeNodeList(node->list);
	if (node->left) {
		freeTree(node->left);
	}
	if (node->right){
		freeTree(node->right);
	}
	free(node);
}