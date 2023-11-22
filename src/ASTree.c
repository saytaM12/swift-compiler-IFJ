#include "LList.h"

AST *ASTreeCreate(int type)
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
    // while (tree->children->head->next != NULL)
    // {
    //     LListDispose(tree->children->head->tree->children);
    // }
    if (tree->children != NULL)
    {
        printf("Disposing %d\n",tree->type);
        LListDispose(tree->children);
    }
    printf("%d\n",tree->type);
    free(tree);
}