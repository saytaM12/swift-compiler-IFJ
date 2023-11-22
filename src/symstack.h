#ifndef SYMSTACK_H
#define SYMSTACK_H

#include "symtable.h"

/**
 * A stack of scopes.
 *
 * The lowest scope is the global scope.
 */
typedef struct
{
    int top;
    unsigned capacity;
    symtable_t *array;
} stack_t;

stack_t *stack_ctor(unsigned capacity);

void stack_dtor(stack_t *stack);

symbol_t *get_symbol(stack_t *stack, char *name);

void add_symbol(stack_t *stack, symbol_t *symbol);

void push(stack_t *stack, symtable_t *symtable);
void pop(stack_t *stack);

#endif