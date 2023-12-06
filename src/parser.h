#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>
#include "lexical.h"
#include "symstack.h"
#include "generator.h"

#define ERROR() do{\
    destroyToken(token);\
    fprintf(stderr, "%d:%d\n", pos.line, pos.col + 1);\
    generator_code_destroy(&code);\
    generator_ins_destroy(ins);\
    }while(0)

#define ENUMTYPE(token)\
    strcmp(token->lexeme, "String") ? strcmp(token->lexeme, "Float") ? Int : Float : String

extern code_t code;

/*
*Get new and delete old token
*FILE* file = file to read from,
*Token* token = structure of read token
*/
Token* new_token(FILE *file, Token* token);


/*
*Start of the program
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_prog();

/*
*Main body
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_main_body(FILE *file, Token* token, stack_t *stack);

/*
*Function declartion
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_func_declare(FILE *file,Token*token,stack_t *stack);

/*
*Parameters of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_param(FILE *file,Token*token,stack_t* stack);

/*
*Type of the parameter of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_param_types(FILE *file,Token*token,stack_t* stack);

/*
*Next parameter of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_next_param(FILE *file, Token* token,stack_t* stack);

/*
*Return type of the function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_function_type(FILE* file, Token* token, char* name, stack_t *stack);

/*
*Body of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_function_body(FILE* file, Token* token, stack_t *stack);

/*
*Body for function/main
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_body(FILE* file, Token* token, stack_t *stack);

/*
*Asign value
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_assign(FILE* file, Token* token,char* name,stack_t *stack);


/*
*Expression
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_expression(FILE* file, Token* token, char* name, stack_t *stack);


/*
*Parameters of the calling function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_call_param(FILE * file, Token * token, char *name,stack_t* stack);

/*
*Types of parameters of the calling function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_call_param_types(FILE* file, Token* token, char *name,stack_t* stack);

void addFunctions(stack_t *stack);
/*
*Next parameter of the calling function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_next_call_param(FILE* file, Token* token, char *name,stack_t* stack);

/*
*Condition in IF
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_if_expression(FILE* file, Token* token, stack_t* stack);

/*
*Body in IF in MAIN
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_if_while_main_body(FILE* file,Token *token, stack_t *stack);

/*
*ELSE in MAIN
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_else_main_body(FILE* file, Token* token, stack_t *stack);

/*
*ELSE in the body of function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_else_function_body(FILE* file, Token* token, stack_t *stack);
#endif 
