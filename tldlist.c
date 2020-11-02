#include <stdio.h>
#include "tldlist.h"

typedef struct tldlist{
    TLDNode *root;
    Date *start_date;
    Date *end_date;
    long size;
    long count;
} TLDList;

typedef struct tldnode{
    TLDNode *left;
    TLDNode *right;
    char *name;
    long count;
    long height;
} TLDNode;

typedef struct tlditerator{
    TLDNode *root;
    TLDNode *current;
} TLDIterator;

TLDNode *tldnode_create(char *element);

void tldnode_destroy(TLDNode *node);

TLDNode *leftrotate(TLDNode *root);

TLDNode *rightrotate(TLDNode *root);

struct TLDNode *tldlist_balance(TLDList *tld, TLDNode *root, TLDNode *node);

long largestValue(long x, long y);

long getHeight(TLDNode *node);


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
    printf("About to free storage\n");

    if (tld != NULL) tldnode_destroy(tld->root);

    printf("Storage freed\n");
};

/**
 * destroy/free a node and recursively destroy/free it's children
 */
void tldnode_destroy(TLDNode *node) {
    if (node != NULL) {
        tldnode_destroy(node->left);
        tldnode_destroy(node->right);
        free(node);
    }
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
    int comp;

    if (date_compare(tld->start_date, d) > 0 || date_compate(tld->end_date, d) < 0) // check if date in range of tree
        return 0;  


    address = strrchr(hostname, '.') + 1; // capture tld from the hostname
    length = strlen(address);
    host = (char *)malloc(sizeof(char)*(length+1));
    host[length] = '\0';
    strcpy(host, address);

    TLDNode *node = (TLDNode *)malloc(sizeof(TLDNode)); // initialise node properties
    if (node != NULL) {
        node->name = host;
        node->count = 1;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
        tld->root = tldlist_balance(tld, tld->root, node);
        tld->count++;
        return 1;
    } else {
        return 0;
    }
};

struct TLDNode *tldlist_balance(TLDList *tld, TLDNode *root, TLDNode *node) {
    
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
        root->count++;                  // new node has same hostname as root so increment root count
    }

    root->height = largestValue(getHeight(root->left), getHeight(root->right))+1;

    long bal = (node == NULL)? 0 : getHeight(node->left) - getHeight(node->right); // check balance of right and left branches for node, >1 for left-heavy, <-1 for right-heavy

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
};

/**
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
    return tld->count;
};

/**
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld){
    
    TLDIterator *iter = (TLDIterator *)malloc(sizeof(TLDIterator));

    if (iter == NULL || tld == NULL) return NULL;

    // initialise iterator properties
    iter->root = tld->root;
    iter->current = tld->root;

    return iter;
};

/**
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter){
    if (iter->current == NULL) 
        return NULL;

    // finish this method!!!!!!
};

/*
* tldlist_iter_destroy destroys the iterator specified by `iter'
*/
void tldlist_iter_destroy(TLDIterator *iter);

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

/**
 *  compare two values and return the largest
 */
long largestValue(long x, long y) {
    return (x > y)? x : y;
};

/** 
 * return height of node or 0 if node is null
 */
long getHeight(TLDNode *node) {
    return (node == NULL)? 0 : node->height;
};

TLDNode *tldnode_create(char *element){
    TLDNode *node = (TLDNode *)malloc(sizeof(TLDNode));
        if (node != NULL) {
            //node->parent = NULL;
            node->left = NULL;
            node->right = NULL;
            node->count = 0;
        }
        return node;
};

/**
 * rotate tree left
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
 * rotate tree right
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