#ifndef ASTREE_H
#define ASTREE_H

#include <stdlib.h>

typedef struct ASTree {
    int type;
    struct LList *children;
    int isNode;
} AST;

AST *ASTreeCreate(int type);
void ASTreeInsert(AST *tree, AST *child);
void ASTreeDispose(AST *tree);

#endif