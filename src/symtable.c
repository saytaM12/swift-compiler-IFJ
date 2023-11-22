#include <stdio.h>
#include "symtable.h"

Stack *Stack_ctor(unsigned capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
    return stack;
}

int main()
{
    return 0;
}
