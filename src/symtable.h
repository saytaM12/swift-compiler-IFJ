#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_SYMTABLE_SIZE 50

typedef enum
{
    undefined_t,
    int_t,
    double_tt,
    string_t,
    bool_t,
    function_t,
    void_t
} Typee;

/**
 * A placeholder for either a variable or a function.
 *
 * If you want to add/modify a field, please make sure to update
 *   1. symbol_t *symbol_ctor
 *   2. symbol_t *symbol_function_ctor
 *   3. symbol_t *symbol_variable_ctor functions.
 */
typedef struct
{
    char *name;
    Typee type;
    bool is_variable; // false means it's a function
    Typee return_type;
    Typee *param_types;
} symbol_t;

symbol_t *symbol_ctor(char *name, Typee type, bool is_variable, Typee return_type, Typee *param_types);

/**
 * A symbol constructor for functions.
 */
symbol_t *symbol_function_ctor(char *name, Typee return_type, Typee *param_types);

/**
 * A symbol constructor for variables.
 */
symbol_t *symbol_variable_ctor(char *name, Typee type);

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

/**
 * Inserts a symbol into the symtable.
 */
void symtable_insert(symtable_t *symtable, symbol_t *symbol);

/**
 * Returns the symbol with the given name.
 */
symbol_t *symtable_lookup(symtable_t *symtable, char *name);

int hash_function(symtable_t *symtable, char *name);

void print_symtable(symtable_t *symtable);

#endif // SYMTABLE_H
