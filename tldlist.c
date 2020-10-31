#include <stdio.h>
#include "tldlist.h"

typedef struct tldlist TLDList;

typedef struct tldnode{
    int key;
    struct TLDNode *left;
    struct TLDNode *right;
    int height;
} TLDNode;

typedef struct tlditerator TLDIterator;



TLDList *tldlist_create(Date *begin, Date *end);

void tldlist_destroy(TLDList *tld);

int tldlist_add(TLDList *tld, char *hostname, Date *d);

long tldlist_count(TLDList *tld);

TLDIterator *tldlist_iter_create(TLDList *tld);

TLDNode *tldlist_iter_next(TLDIterator *iter);

void tldlist_iter_destroy(TLDIterator *iter);

char *tldnode_tldname(TLDNode *node);

long tldnode_count(TLDNode *node);