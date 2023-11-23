#include <stdio.h>

#include "symtable.h"

symbol_t *symbol_ctor(char *name, Type type, void *value, bool is_variable, Type return_type, Type *param_types)
{
    symbol_t *symbol = (symbol_t *)malloc(sizeof(symbol_t));

    symbol->name = name;
    symbol->type = type;
    symbol->value = value;
    symbol->is_variable = is_variable;
    symbol->return_type = return_type;
    symbol->param_types = param_types;

    return symbol;
}

symbol_t *symbol_function_ctor(char *name, Type return_type, Type *param_types)
{
    return symbol_ctor(name, function_t, NULL, false, return_type, param_types);
}

symbol_t *symbol_variable_ctor(char *name, Type type, void *value)
{
    return symbol_ctor(name, type, value, true, undefined_t, NULL);
}

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

// TODO: Come up with a better hash function
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

    // Does this create any problems?
    free(symbol);
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

void symbol_dtor(symbol_t *symbol)
{
    free(symbol);
}

void testing_function()
{
    printf("testing\n");
}

/*int main()
{
    symtable_t *symtable = symtable_ctor();

    int testing = 5;
    symbol_t *symbol = symbol_ctor("testing", int_t, &testing, true, undefined_t, NULL);
    symbol_t *symbol2 = symbol_ctor("testing_function", function_t, &testing_function, false, void_t, NULL);

    symtable_insert(symtable, symbol);
    symtable_insert(symtable, symbol2);

    print_symtable(symtable);

    char *found = symtable_lookup(symtable, "testing")->name;
    printf("%s\n", found);

    char *test = symtable_lookup(symtable, "testing_function")->name;
    printf("%s\n", test);

    symtable_dtor(symtable);
    return 0;
}*/
