#include "symstack.h"

stack_t *stack_ctor()
{
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    stack->capacity = DEFAULT_STACK_SIZE;
    stack->top = -1;
    stack->array = (symtable_t *)malloc(stack->capacity * sizeof(symtable_t));
    return stack;
}

void stack_dtor(stack_t *stack)
{
    free(stack->array);
    free(stack);
}

symbol_t *get_symbol(stack_t *stack, char *name)
{
    for (int i = stack->top; i >= 0; i--)
    {
        symbol_t *symbol = symtable_lookup(&stack->array[i], name);
        if (symbol != NULL)
        {
            return symbol;
        }
    }
    return NULL;
}

void add_symbol(stack_t *stack, symbol_t *symbol)
{
    symtable_insert(&stack->array[stack->top], symbol);
}

symbol_t *get_symbol_top(stack_t *stack, char *name)
{
    symbol_t *symbol = symtable_lookup(&stack->array[stack->top], name);
    if (symbol != NULL)
    {
        return symbol;
    }
    return NULL;
}

void push_new_scope(stack_t *stack)
{
    if (stack->top == stack->capacity - 1)
    {
        // resize the stack
        stack->capacity *= 2;
        stack->array = (symtable_t *)realloc(stack->array, stack->capacity * sizeof(symtable_t));

        if (stack->array == NULL)
        {
            printf("ERROR: Stack overflow. You tried to push to a full stack and realloc has failed.\n");
            exit(1);
        }
    }
    symtable_t *symtable = symtable_ctor();

    stack->array[++stack->top] = *symtable;
}

void pop_scope(stack_t *stack)
{
    if (stack->top == -1)
    {
        printf("ERROR: Stack underflow. You tried to pop an empty stack.\n");
        exit(1);
    }
    // symtable_dtor(&stack->array[stack->top]); // TODO: this causes a double free error
    stack->top--;
}