/**
 * HOW TO USE THIS?
 *      1. Create a stack_t *stack = stack_ctor();
 *      2. Push a new scope with push_new_scope(stack); This adds a new scope on top.
 *      3. Create symbols with symbol_variable_ctor or symbol_function_ctor.
 *      4. Add symbols to the scope with add_symbol(stack, symbol);
 *      5. Get symbols with get_symbol(stack, name);
 *      6. Pop the scope with pop_scope(stack);
 *      7. Destroy the stack with stack_dtor(stack);
 */

#ifndef SYMSTACK_H
#define SYMSTACK_H

#include "symtable.h"

#define DEFAULT_STACK_SIZE 50

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

stack_t *stack_ctor();

void stack_dtor(stack_t *stack);

/**
 * Returns the symbol with the given name.
 *
 * If the symbol is not found, returns NULL.
 */
symbol_t *get_symbol(stack_t *stack, char *name);

symbol_t *get_symbol_top(stack_t *stack, char *name);

/**
 * Adds a symbol to the top scope.
 */
void add_symbol(stack_t *stack, symbol_t *symbol);

/**
 * Pushes a new scope on top of the stack.
 */
void push_new_scope(stack_t *stack);

/**
 * Pops the top scope from the stack.
 */
void pop_scope(stack_t *stack);

#endif