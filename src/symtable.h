#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_SYMTABLE_SIZE 20

typedef enum
{
    undefined_t,
    int_t,
    double_t,
    string_t,
    bool_t,
    function_t
} Type;

/**
 * A placeholder for either a variable or a function.
 *
 *
 */
typedef struct
{
    char *name;
    Type type;
    void *value;
    bool is_variable; // false means it's a function
} symbol_t;

symbol_t *symbol_ctor(char *name, Type type, void *value, bool is_variable);

void symbol_dtor(symbol_t *symbol);

/**
 * A symtable_t (= scope of variables) is a collection of symbols.
 *
 * It is implemented as a hash table with open addressing.
 */
typedef struct
{
    unsigned capacity;
    symbol_t *array;
} symtable_t;

symtable_t *symtable_ctor();

void symtable_dtor(symtable_t *symtable);

void symtable_insert(symtable_t *symtable, symbol_t *symbol);

symbol_t *symtable_lookup(symtable_t *symtable, char *name);

int hash_function(symtable_t *symtable, char *name);

void print_symtable(symtable_t *symtable);

#endif // SYMTABLE_H
