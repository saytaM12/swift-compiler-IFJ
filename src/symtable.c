#include <stdio.h>

#include "symtable.h"

symtable_t *symtable_ctor()
{
    symtable_t *symtable = (symtable_t *)malloc(sizeof(symtable_t));

    symtable->capacity = DEFAULT_SYMTABLE_SIZE;
    symtable->array = (symbol_t *)malloc(symtable->capacity * sizeof(symbol_t));

    return symtable;
}

void symtable_dtor(symtable_t *symtable)
{
    free(symtable->array);
    free(symtable);
}

int hash_function(symtable_t *symtable, char *name)
{
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++)
    {
        hash += name[i];
    }
    return hash % symtable->capacity;
}

void symtable_insert(symtable_t *symtable, symbol_t *symbol)
{
    int index = hash_function(symtable, symbol->name);

    while (symtable->array[index].name != NULL)
    {
        index++;
        index %= symtable->capacity;
    }

    symtable->array[index] = *symbol;
}

symbol_t *symtable_lookup(symtable_t *symtable, char *name)
{
    int index = hash_function(symtable, name);

    while (symtable->array[index].name != NULL)
    {
        if (strcmp(symtable->array[index].name, name) == 0)
        {
            return &symtable->array[index];
        }
        index++;
        index %= symtable->capacity;
    }

    return NULL;
}

void print_symtable(symtable_t *symtable)
{
    for (int i = 0; i < symtable->capacity; i++)
    {
        if (symtable->array[i].name != NULL)
        {
            printf("%d: %s\n", i, symtable->array[i].name);
        }
    }
}

symbol_t *symbol_ctor(char *name, Type type, void *value, bool is_variable)
{
    symbol_t *symbol = (symbol_t *)malloc(sizeof(symbol_t));

    symbol->name = name;
    symbol->type = type;
    symbol->value = value;
    symbol->is_variable = is_variable;

    return symbol;
}

void symbol_dtor(symbol_t *symbol)
{
    free(symbol);
}

void testing_function()
{
    printf("testing\n");
}

int main()
{
    symtable_t *symtable = symtable_ctor();

    int testing = 5;
    symbol_t *symbol = symbol_ctor("testing", int_t, &testing, true);
    symbol_t *symbol2 = symbol_ctor("testing_function", function_t, &testing_function, false);

    symtable_insert(symtable, symbol);
    symtable_insert(symtable, symbol2);

    print_symtable(symtable);

    char *found = symtable_lookup(symtable, "testing")->name;
    printf("%s\n", found);

    char *test = symtable_lookup(symtable, "testing_function")->name;
    printf("%s\n", test);

    symbol_dtor(symbol);
    symtable_dtor(symtable);
    return 0;
}
