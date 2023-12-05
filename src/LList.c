#include "LList.h"

LList* LListCreate() {
    LList* list = (LList*) malloc(sizeof(LList));
    list->head = NULL;
    list->active = NULL;
    return list;
}


void LListDispose(LList* list) {
    LListElement* node = list->head;
    while (node != NULL) {
        if (node->tree!=NULL)
        {
            // printf("Disposing %s\n",node->tree->type);
            ASTreeDispose(node->tree);

        }
        LListElement* next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

void LListInsert(LList* list, AST* tree) {
        if (list->head == NULL)
        {
            LListElement* tmp = (LListElement*) malloc(sizeof(LListElement));
            if (tmp == NULL)
            {
                printf("Error: malloc failed\n");
            }
            tmp->tree = tree;
            tmp->next = NULL;
            list->head = tmp;
            list->active = tmp;
            return;            
        }
        else
        {
            LListElement* tmp = (LListElement*) malloc(sizeof(LListElement));
            if (tmp == NULL)
            {
                printf("Error: malloc failed\n");
            }
            while (list->active->next != NULL)
            {
                list->active = list->active->next;
            }
            
            tmp->tree = tree;
            tmp->next = NULL;
            list->active->next = tmp;
            return;
        }
}