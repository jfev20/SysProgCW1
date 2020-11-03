#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tldlist.h"

typedef struct tldlist{
    TLDNode *root;      // Root node of AVL tree 
    Date *start_date;   // starting date for tree
    Date *end_date;     // final date for tree
    long size;          // total number of nodes in tree
    long count;         // number of accepted TLDs
} TLDList;

typedef struct tldnode{
    TLDNode *left;      // pointer to left child node
    TLDNode *right;     // pointer to right child node
    char *name;         // TLD name
    long count;         // number of TLDs of the same name
    long height;        // height of node in the tree
} TLDNode;

typedef struct tlditerator{
    TLDList *tree;      // copy of AVL tree
    TLDNode **nodes;    // array of all nodes
    long size;          // total number fo nodes (taken from TLDList)
    int index;          // index of current node
} TLDIterator;

/**
 * |========[ORDER OF FUNCTIONS]========|
 * |    HELPER METHOD INITIALISATION    |
 * |    LIST FUNCTIONS                  |
 * |    NODE FUNCTIONS                  |
 * |    ITERATOR FUNCTIONS              |
 * |    ROTATION FUNCTIONS              |
 * |    OTHER HELPER FUNCTIONS          |
 * |====================================|
 */

//------------------------------------[HELPER METHOD INITIALISATION]-------------------------------------------------

/**
 * extra helper methods
 */
TLDNode *tldnode_create(char *name);
void tldnode_destroy(TLDNode *node);
TLDNode *leftrotate(TLDNode *root);
TLDNode *rightrotate(TLDNode *root);
TLDNode *tldlist_balance(TLDList *tld, TLDNode *root, TLDNode *node);
long largestValue(long x, long y);
long getHeight(TLDNode *node);
void populate_iter (TLDIterator *iter, TLDNode *node, int *i);

//------------------------------------------[LIST FUNCTIONS]------------------------------------------------------

/**
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
*/
TLDList *tldlist_create(Date *begin, Date *end){
    TLDList *list = (TLDList *)malloc(sizeof(TLDList)); // allocate memory for list

    if (list != NULL) {         // initialise list properties
        list->root = NULL;
        list->count = 0;
        list->size = 0;
        list->start_date = date_duplicate(begin);
        list->end_date = date_duplicate(end);
    }

    return list;
};

/**
 * destroy a list structure starting with the root node, calling the destroy node function
 */
void tldlist_destroy(TLDList *tld){
    if (tld != NULL) {
        tldnode_destroy(tld->root);     // call node destroyer function, passing it the root to destroy all nodes
    }
    // preventing memory leaks
    date_destroy(tld->start_date);
    date_destroy(tld->end_date);
    free(tld);
};

/**
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d){

    char *address;
    char *host;
    int length;

    // check if date in range of tree
    if (date_compare(tld->start_date, d) > 0 || date_compare(tld->end_date, d) < 0) 
        return 0;  

    // capture tld from the hostname
    address = strrchr(hostname, '.') + 1;           // removes dot before TLD
    length = strlen(address);
    host = (char *)malloc(sizeof(char)*(length+1));
    host[length] = '\0';
    strcpy(host, address);

    // create node and initialise properties
    TLDNode *node = tldnode_create(host);

    if (node != NULL) {
        tld->root = tldlist_balance(tld, tld->root, node); // set new root to root returned by balance tree function
        tld->count++;
        return 1;
    } else {
        // preventing memory leaks
        free(host);
        free(node);
        return 0;
    }
};

/**
 * method to balance out AVL tree if too many nodes on one side
 */
TLDNode *tldlist_balance(TLDList *tld, TLDNode *root, TLDNode *node) {
    
    // if tldlist is empty, increment list size and return node
    if (root == NULL) { 
        tld->size++;
        return node;
    }
    
    if (strcmp(node->name, root->name) < 0) {                   // compare new node with root node
        root->left = tldlist_balance(tld, root->left, node);    // continue comparison down left 'branch'
    } else if (strcmp(node->name, root->name) > 0) {
        root->right = tldlist_balance(tld, root->right, node);  // continue comparison down right 'branch'
    } else {
        root->count++;                                          // new node has same hostname as root so increment root count
        // preventing memory leaks
        tldnode_destroy(node);
    }

    root->height = largestValue(getHeight(root->left), getHeight(root->right))+1;

    // check balance of right and left branches for node, >1 for left-heavy, <-1 for right-heavy
    long bal = (node == NULL)? 0 : getHeight(node->left) - getHeight(node->right); 

    if (bal > 1) {                                              // if tree is left branch heavy...
        if (strcmp(node->name, root->left->name) < 0) {         // ...and if the new node goes on the right...
            return rightrotate(root);                           // ...rotate tree right
        } else if (strcmp(node->name, root->left->name) > 0) {  // ..else if the new node goes on the left...
            root->left = leftrotate(root->left);                // ...rotate left...
            return rightrotate(root);                           // ...then rotate right
        }
    } else if (bal < -1) {                                      // if the tree is reight branch heavy...
        if (strcmp(node->name, root->right->name) > 0) {        // ...and if the new node goes on the left...
            return leftrotate(root);                            // ...rotate tree left
        } else if (strcmp(node->name, root->right->name) < 0) { // ...else if the new node goes on the right
            root->right = rightrotate(root->right);             // ...rotate right...
            return leftrotate(root);                            // ... then rotate left
        }
    }
    return root;
};

/**
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
    return tld->count;
};

//--------------------------------------[NODE FUNCTIONS]-----------------------------------------------------

/**
 * helper function to create new node for the AVL tree
 */
TLDNode *tldnode_create(char *name){
    TLDNode *node = (TLDNode *)malloc(sizeof(TLDNode));
        //initialise node properties
        if (node != NULL) {
            node->left = NULL;
            node->right = NULL;
            node->count = 1;
            node->name = name;
            node->height = 1;
        }
        return node;
};

/**
 * destroy/free a node and recursively destroy/free it's children
 */
void tldnode_destroy(TLDNode *node) {
    if (node != NULL) {
        tldnode_destroy(node->left);    // recursively destroy left child nodes
        tldnode_destroy(node->right);   // recursively destroy right child nodes

        // preventing memory leaks
        free(node->name);   
        free(node);
        node = NULL;
    }
};

/**
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node){
    return node->name;
};

/**
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node){
    return node->count;
};


//-------------------------------------------[ITERATOR FUNCTIONS]-----------------------------------------------


/**
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld){
    
    TLDIterator *iter = (TLDIterator *)malloc(sizeof(TLDIterator));

    if (iter == NULL || tld == NULL) {
        free(iter); // preventing memory leaks
        return NULL;
    }

    // initialise iterator properties
    iter->tree = tld;
    iter->size = tld->size;
    iter->index = 0;
    iter->nodes = (TLDNode **)malloc(sizeof(TLDNode *) * iter->size);

    if (iter->nodes == NULL) {
        tldlist_iter_destroy(iter);
        return NULL;
    }

    // recursively insert nodes to tree
    int i = 0;
    if (iter->size != 0) 
        populate_iter(iter, iter->tree->root, &i);

    return iter;
};

/**
 * helper function to populate iterator with nodes from AVL tree
 */
void populate_iter (TLDIterator *iter, TLDNode *node, int *i) {
    if (node->left != NULL)
        populate_iter(iter, node->left, i);     // recursively iterate through left children

    *(iter->nodes + (*i)++) = node;             // add node to iter node array

    if (node->right != NULL)
        populate_iter(iter, node->right, i);    // recursively iterate through right children
};

/**
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter){
    // check if it has reached the end of the tree
    if (iter->index == iter->size) 
        return NULL;

    TLDNode *next = *(iter->nodes + iter->index++); // pointer to the next node in array

    return next;
};

/*
* tldlist_iter_destroy destroys the iterator specified by `iter'
*/
void tldlist_iter_destroy(TLDIterator *iter){
    free(iter->nodes);
    free(iter);
};

//-----------------------------------------[ROTATION FUNCTIONS]-------------------------------------------

/**
 * helper function to rotate tree left
 */
TLDNode *leftrotate(TLDNode *root) {
    TLDNode *newRoot = root->right; // right child of current root becomes new root
    TLDNode *n = newRoot->left;
    newRoot->left = root;           // old root becomes left child of new root
    root->right = n; 

    root->height = largestValue(getHeight(root->left), getHeight(root->right))+1;
    newRoot->height = largestValue(getHeight(newRoot->left), getHeight(newRoot->right))+1;

    return newRoot;
}

/**
 * helper function to rotate tree right
 */
TLDNode *rightrotate(TLDNode *root) {
    TLDNode *newRoot = root->left;  // left child of current root becomes new root
    TLDNode *n = newRoot->right;
    newRoot->right = root;          // old root becomes right child of new root
    root->left = n;

    root->height = largestValue(getHeight(root->left), getHeight(root->right))+1;
    newRoot->height = largestValue(getHeight(newRoot->left), getHeight(newRoot->right))+1;

    return newRoot;
}

//-------------------------------------[OTHER HELPER FUNCTIONS]----------------------------------------

/**
 *  helper function to return the largest of two values
 */
long largestValue(long x, long y) {
    return (x > y)? x : y;
};

/** 
 * helper function to return height of node in tree or 0 if node is null
 */
long getHeight(TLDNode *node) {
    return (node == NULL)? 0 : node->height;
};