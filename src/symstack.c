#include <stdio.h>

#include "symstack.h"

stack_t *stack_ctor(unsigned capacity)
{
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
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

void push(stack_t *stack, symtable_t *symtable)
{
    if (stack->top == stack->capacity - 1)
    {
        // TODO: resize
        printf("Stack overflow\n");
        exit(1);
    }
    stack->array[++stack->top] = *symtable;
}

void pop(stack_t *stack)
{
    if (stack->top == -1)
    {
        printf("ERROR: Stack underflow. You tried to pop an empty stack.\n");
        exit(1);
    }
    stack->top--;
}

int main()
{
    return 0;
}