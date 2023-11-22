#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <stdio.h>
#include "ASTree.h"

typedef struct LListElement {
    struct ASTree *tree;    
    struct LListElement *next;
} LListElement;

typedef struct LList {
    struct LListElement *head;
    struct LListElement *active;
} LList;

LList *LListCreate();
void LListDispose(LList *list);
void LListInsert(LList *list, AST *tree);

#endif