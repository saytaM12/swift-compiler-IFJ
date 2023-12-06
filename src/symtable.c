#include "symtable.h"
#include "generator.h"

symbol_t *symbol_ctor(char *name, Typee type, bool is_variable, Typee return_type, Typee *param_types, int size)
{
    symbol_t *symbol = (symbol_t *)malloc(sizeof(symbol_t));

    // copy the name into a new char *
    int name_length = strlen(name);
    symbol->name = (char *)malloc((name_length + 1) * sizeof(char));
    strcpy(symbol->name, name);

    symbol->type = type;
    symbol->is_variable = is_variable;
    symbol->return_type = return_type;
    symbol->param_types = param_types;
    symbol->size = size;
    return symbol;
}

symbol_t *symbol_function_ctor(char *name, Typee return_type, Typee *param_types, int size)
{
    return symbol_ctor(name, function_t, false, return_type, param_types,size);
}

symbol_t *symbol_variable_ctor(char *name, Typee type)
{
    return symbol_ctor(name, type, true, undefined_t, NULL, 0);
}

symtable_t *symtable_ctor()
{
    symtable_t *symtable = (symtable_t *)malloc(sizeof(symtable_t));

    symtable->capacity = DEFAULT_SYMTABLE_SIZE;
    symtable->array = (symbol_t *)malloc(symtable->capacity * sizeof(symbol_t));

    for (int i = 0; i < symtable->capacity; i++)
    {
        symtable->array[i].name = NULL;
    }

    return symtable;
}

void symtable_dtor(symtable_t *symtable)
{
    if (!symtable) {
        return;
    }
    free(symtable->array);
    //free(symtable);
}

int hash_function(symtable_t *symtable, char *name)
{
    unsigned h = 0; // musí mít 32 bitů
    const unsigned char *p;
    for (p = (const unsigned char *)name; *p != '\0'; p++)
        h = 65599 * h + *p;

    return h % symtable->capacity;
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
    if (!symbol)
        return;
    free(symbol->name);
    free(symbol);
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
