#ifndef GENERATOR_H
#define GENERATOR_H
#include <stdio.h>
#include <stdlib.h>

/*
 * Single node of double linked list of lines of code IFJcode23
 */
typedef struct Line{
     struct Line *prev;
     struct Line *next;
     char *line;
} line_t;

/*
 * Double linked list structure with lines of code IFJcode23
 */
typedef struct {
    line_t *first;
    line_t *last;
} code_t;

typedef enum {
    funDef = 1,
    funCal = 2,
    varDef = 3,
    assign = 4,
    whileLoop = 5,
    ifExpr = 6,
} instructionType_e;

typedef enum {
    Int = 1,
    Float = 2,
    String = 3,
} varialbeType_e;

// function definition ///////////////////
                                        //
struct funcDefParam {                   //
    char *name;                         //
    char *id;                           //
    varialbeType_e type;                //
};                                      //
                                        //
struct funDef_t {                       //
    varialbeType_e type;                //
    char *name;                         //
    struct funcDefParam *parameters;    //
};                                      //
                                        //
//////////////////////////////////////////

// function Call /////////////////////////
                                        //
struct funcCallParam {                  //
    varialbeType_e type;                //
    char *value;                        //
};                                      //
                                        //
struct funCal_t {                       //
    char *name;                         //
    struct funcCallParam *parameters;   //
};                                      //
                                        //
//////////////////////////////////////////

// variable definition ///////////////////
                                        //
struct varDef_t {                       //
    char *name;                         //
    varialbeType_e type;                //
    char *value;                        //
};                                      //
                                        //
//////////////////////////////////////////

// assignment ////////////////////////////
                                        //
struct assign_t {                       //
    char *from;                         //
    char *to;                           //
};                                      //
                                        //
//////////////////////////////////////////

// while loop ////////////////////////////
                                        //
struct whileLoop_t {                    //
};                                      //
                                        //
//////////////////////////////////////////

// if expression /////////////////////////
                                        //
struct ifExpr_t {                       //
};                                      //
                                        //
//////////////////////////////////////////


typedef struct {
    instructionType_e instructionType;
    union {
        struct funDef_t funDef;
        struct funCal_t funCal;
        struct varDef_t varDef;
        struct assign_t assign;
        //struct whileLoop_t whileLoop;
        //struct ifExpr_t ifExpr;
    };
} instruction_t;

/*
 * Inicializes empty code_t structure
 * this will later be used to store final code
 */
void generator_init(code_t *code);

/*
 * Frees every elemet and bring the structure to state after init
 */
void generator_destroy(code_t *code);

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

#endif // GENERATOR_H
