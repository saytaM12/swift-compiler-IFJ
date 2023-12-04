/**
 * This is a temporary fiel to show how to use the SYMTABLE module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "symtable.h"
#include "symstack.h"

int main2()
{

    printf("-> starting example...\n");
    // Creates a stack of scopes. NOTE: One stack per program!
    stack_t *stack = stack_ctor();

    // Create variable symbols
    printf("-> symbol ctor...\n");

    int x = 6;
    symbol_t *symbol = symbol_variable_ctor("x", int_t);
    symbol_t *symbol2 = symbol_variable_ctor("y", int_t);

    printf("-> function ctor...\n");
    // Create function symbols
    Type param_types[2] = {int_t, int_t};
    symbol_t *symbol3 = symbol_function_ctor("add", int_t, param_types); // an example function can be seen below

    printf("-> pushing new scope...\n");
    // Create a scope
    push_new_scope(stack);

    // Add symbols to the scope
    add_symbol(stack, symbol);
    add_symbol(stack, symbol2);
    add_symbol(stack, symbol3);

    // NOTE: Here, the stack looks as follows:
    //       STACK:
    //       -----------
    //       | scope 1 | <-- top
    //       -----------
    //
    //       scope 1 has 3 symbols: x, y, add

    // Get a symbol from the scope
    printf("-> getting symbol...\n");
    symbol_t *found = get_symbol(stack, "x");

    printf("Found variable with the name: %s\n", found->name);

    // Get a function symbol from the scope
    symbol_t *found2 = get_symbol(stack, "add");

    printf("Found function with name: %s\n", found2->name);
    printf("Its return type is: %d\n", found2->return_type);
    printf("---------\n");

    push_new_scope(stack);

    int z = 10;
    // try to add a symbol with the same name as the one in the previous scope
    symbol_t *symbol4 = symbol_variable_ctor("x", int_t);

    add_symbol(stack, symbol4);

    // NOTE: Here, the stack looks as follows:
    //       STACK:
    //       -----------
    //       | scope 2 | <-- top
    //       -----------
    //       | scope 1 |
    //       -----------
    //
    //       scope 1 has 3 symbols: x, y, add
    //       scope 2 has 1 symbol: x
    //       Since scope 2 is the topmost scope.

    // try to get x
    symbol_t *found3 = get_symbol(stack, "x");

    printf("Found variable with the name: %s\n", found3->name);
    // Here, we get the topmost x.
    printf("---------\n");

    pop_scope(stack);

    // try to get x
    symbol_t *found4 = get_symbol(stack, "x");

    printf("Found variable with the name: %s\n", found4->name);
    // Here, the value of x is 6, because in the topmost scope, x is 6 again.

    stack_dtor(stack);
    return 0;
}

int add(int x, int y)
{
    return x + y;
}
