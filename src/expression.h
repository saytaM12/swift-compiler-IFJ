#ifndef EXPRESSIN_H
#define EXPRESSIN_H
#include "lexical.h"
typedef enum {R, S, Eq, Err, Fin}prec_action; // R = reduce, S = shift, Eq = equal, Err = error, Fin = final
typedef enum {PlusMinus, MultiplyDivide, Rel, ExMark, QuestionMark, OpenBracket, CloseBracket, Identifier, Dollar, Expression}prec_index;
// +-   */   !=<>   !   ??   (   )   i   $
typedef enum {num, str, doub, boo, nil}type;

typedef struct expression_value
{
    type type;
    prec_action action;
    prec_index index;
    char *value;
    struct expression_value *left;
    struct expression_value *right;
}expression_value;

typedef struct expression_element
{
    expression_value *value;
    struct expression_element *next;
    struct expression_element *prev;
    
}expression_element;

typedef struct expression_list
{
    expression_element *head;
    expression_element *active;
}expression_list;

// typedef struct expression_value
// {
//     type_index type;
//     prec_action action;
//     AST* tree;
//     prec_index index;
// }expression_value;

// typedef struct expression_value
// {
//     char *value;
//     struct  expression_value *left;
//     struct  expression_value *right;
//     prec_action action;
//     prec_index type;
// }expression_value;

// typedef struct expression_element
// {
//     expression_value* value;
//     struct expression_element* next;    
// }expression_element;

// typedef struct expression_list
// {
//     expression_element* head;
//     expression_element* active;
// }expression_list;
void reduce(expression_list* stack);
expression_list* expression_list_create();
void expression_list_insert(expression_list* list, expression_value *value);
void expression_list_dispose(expression_list* list);
expression_value *expression_list_pop(expression_list *list);
void disposeValue(expression_value* value);
void printExprList(expression_list* stack);
void printValue(expression_value* value, int level);
expression_value *expression_last(expression_list *list);
expression_value* expression_value_create(Token* token);
expression_list *bottomUp(Token *token);
// prec_index getIndex(char* type);
#endif