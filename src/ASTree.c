#include "LList.h"

AST *ASTreeCreate(char *type)
{
    AST *tree = (AST *)malloc(sizeof(AST));
    tree->type = type;
    tree->children = NULL;
    tree->isNode = 0;
    return tree;
}

void ASTreeInsert(AST *tree, AST *child)
{
    if (tree->children == NULL)
    {
        LList *list = LListCreate();
        LListInsert(list, child);
        tree->children = list;
    }
    else
    {
        LListInsert(tree->children, child);
    }
    tree->isNode=1;
}

void ASTreeDispose(AST *tree)
{
    if (tree->children != NULL)
    {
        // printf("Disposing %s\n",tree->type);
        LListDispose(tree->children);
    }
    // printf("%s\n",tree->type);
    if (tree->type != NULL)
    {
        free(tree->type);
    }
    free(tree);
}