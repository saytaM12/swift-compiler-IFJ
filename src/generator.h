#ifndef GENERATOR_H
#define GENERATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Single node of double linked list of lines of code IFJcode23
 */
typedef struct Line
{
    struct Line *prev;
    struct Line *next;
    char *line;
} line_t;

/*
 * Double linked list structure with lines of code IFJcode23
 */
typedef struct
{
    line_t *first;
    line_t *last;
} code_t;

typedef enum
{
    funDef = 1,
    funCal = 2,
    varDef = 3,
    assign = 4,
    whileLoop = 5,
    ifExpr = 6,
} instructionType_e;

typedef enum
{
    Int = 1,
    Float = 2,
    String = 3,
} varialbeType_e;

const char *typeLookup[] = {
    "int",
    "float",
    "string",
};

typedef struct Scope{
    struct Scope *next;
    char *name;
    int offset;
} scope_t;

// function definition ///////////////////

struct funDefParam {
    char *name;
    char *id;
    varialbeType_e type;
};

struct funDef_t {
    varialbeType_e type;
    char *name;
    int paramNum;
    struct funDefParam **parameters;
};

// function Call /////////////////////////

struct funCallParam {
    varialbeType_e type;
    char *value;
};

struct funCal_t {
    char *name;
    struct funCallParam **parameters;
};

// variable definition ///////////////////
struct varDef_t
{
    char *name;
    varialbeType_e type;
    char *value;
    bool local;
};

// assignment ////////////////////////////
struct assign_t
{
    char *from;
    char *to;
};

// while loop ////////////////////////////
struct whileLoop_t
{
};

// if expression /////////////////////////
struct ifExpr_t
{
};

typedef struct {
    scope_t *currScope;
    int totalOffset;
    instructionType_e instructionType;
    union
    {
        struct funDef_t funDef;
        struct funCal_t funCal;
        struct varDef_t varDef;
        struct assign_t assign;
        // struct whileLoop_t whileLoop;
        // struct ifExpr_t ifExpr;
    };
} instruction_t;

extern instruction_t *ins;

/*
 * Inicializes empty code_t structure
 * this will later be used to store final code
 */
code_t generator_code_init();



/*
 * Inicializes empty instruction_t structure
 * this will later be used to store a line of IFJ23 text
 */
instruction_t *generator_ins_init();

/*
 * adds a scope to scope buffer and sets it's offset
 */
void generator_ins_push_scope(instruction_t *ins, int offset, char *name);

/*
 * increses offset of current scope and totalOffset
 */
void generator_ins_add_to_offset(instruction_t *ins, int offset);

/*
 * returns a scope_t structure, which is at the top of the scope buffer
 */
char *generator_ins_get_current_scope_name(instruction_t ins);

/*
 * pops a scope from scope buffer
 */
void generator_ins_pop_scope(instruction_t *ins);



/*
 * Frees every element and brings the structure to state after init
 */
void generator_code_destroy(code_t *code);

/*
 * Frees every element and brings the structure to state after init
 */
void generator_ins_destroy(instruction_t *ins);

/*
 * Adds a line to the end of the structure
 */
int generator_addLineEnd(code_t *code, char *line);

/*
 * Adds a line to the structre `offset` lines from the end
 */
int generator_addLineFromEnd(code_t *code, char *line, int offset);

/*
 * Write all of the lines in `code` to the file
 */
void generator_write(FILE *file, code_t code);

/*
 * Translates a line of IFJ23 into IFJcode23
 */
void generator_translate();

#endif // GENERATOR_H
