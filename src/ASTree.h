#ifndef ASTREE_H
#define ASTREE_H

#include <stdlib.h>

typedef struct ASTree {
    char *type;
    struct LList *children;
    int isNode;
} AST;

AST *ASTreeCreate(char *type);
void ASTreeInsert(AST *tree, AST *child);
void ASTreeDispose(AST *tree);

#endif