#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>

/**
 * A placeholder for either a variable or a function.
 *
 *
 */
typedef struct
{
    char *name;
    int type;
    int scope;
    int offset;
} Symbol;

typedef struct
{
    int parent;
    int size;
    int offset;
    Symbol *symbols;
} Scope;

typedef struct
{
    int top;
    unsigned capacity;
    int *array;
} Stack;

Stack *Stack_ctor(unsigned capacity);

void Stack_dtor(Stack *stack);

#endif // SYMTABLE_H
