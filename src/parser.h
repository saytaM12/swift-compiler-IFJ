#ifndef PARSER_H
#define PARSER_H

#include "lexical.h"

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
int parse_main_body(FILE *file, Token* token);

/*
*Function declartion
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_func_declare(FILE *file,Token*token);

/*
*Parameters of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_param(FILE *file,Token*token);

/*
*Type of the parameter of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_param_types(FILE *file,Token*token);

/*
*Next parameter of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_next_param(FILE *file, Token* token);

/*
*Return type of the function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_function_type(FILE* file, Token* token);

/*
*Body of the declared function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_function_body(FILE* file, Token* token);

/*
*Body for function/main
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_body(FILE* file, Token* token);

/*
*Asign value
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_assign(FILE* file, Token* token);


/*
*Expression
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_expression(FILE* file, Token* token);


/*
*Parameters of the calling function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_call_param(FILE * file, Token * token);

/*
*Types of parameters of the calling function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_call_param_types(FILE* file, Token* token);

/*
*Next parameter of the calling function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_next_call_param(FILE* file, Token* token);

/*
*Condition in IF
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_if_expression(FILE* file, Token* token);

/*
*Body in IF in MAIN
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_if_while_main_body(FILE* file,Token *token);

/*
*ELSE in MAIN
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_else_main_body(FILE* file, Token* token);

/*
*ELSE in the body of function
*FILE* file = file to read from,
*Token* token = structure of read token
*/
int parse_else_function_body(FILE* file, Token* token);
#endif 