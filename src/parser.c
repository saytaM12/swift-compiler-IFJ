#include "parser.h"
#include "symstack.h"
#include "symtable.h"


Typee param_types[2];

// Get new and delete old token
Token* new_token(FILE* file, Token* token){
    destroyToken(token);
    return getToken(file);
}

void addSymbol(Token* token, char* name, stack_t *stack){
    symbol_t *symbol;
    switch (token->type) {
            case identifier:
                printf("(identifier)   ");
                break;
            case keyword:
                printf("chyba");
                break;
            case variableType:
                printf("chyba");
                break;
            case number:
                symbol = symbol_variable_ctor(name, int_t);
                add_symbol(stack, symbol);
                break;
            case operation:
                printf("(operation)    ");
                break;
            case singleChars:
                printf("(singleChars)  ");
                break;
            case string:
                symbol = symbol_variable_ctor(name, string_t);
                add_symbol(stack, symbol);
                break;
            case unknown:
                printf("(unknown)      ");
                break;
        }
}

// Start of the program
int parse_prog(){
    FILE* file = fopen("input.swift", "r");
    Token* token = NULL;
    stack_t *stack = stack_ctor();
    push_new_scope(stack);
    return parse_main_body(file,token, stack);
}

// Main body
// <MAIN_BODY> 
int parse_main_body(FILE *file, Token* token, stack_t *stack){
    token = getToken(file);
    // -> eps
    if(token->lexeme[0] == EOF){
        symbol_t *found = get_symbol(stack, "z");
        if(found!=NULL)
            printf("Found variable with the name: %s\n", found->name);
        pop_scope(stack);
        stack_dtor(stack);
        destroyToken(token);
        return 0;
    }
    // -> func <FUNCTION_DECLARE>
    if(!strcmp(token->lexeme, "func")){
        printf("\nfunc\n");
        return parse_func_declare(file,token,stack) || parse_main_body(file,token,stack);
    }
    // -> if <IF_WHILE_EXPRESSION> <IF_WHILE_MAIN_BODY> <ELSE_MAIN_BODY> <MAIN_BODY>
    if(!strcmp(token->lexeme,"if")){
        printf("if\n");
        return parse_if_expression(file,token) || parse_if_while_main_body(file,token,stack) || parse_else_main_body(file,token,stack) || parse_main_body(file,token, stack);
    }
    // -> while [expression] <IF_WHILE_MAIN_BODY> <MAIN_BODY>
    // TODO
    if(!strcmp(token->lexeme,"while")){
        printf("while\n");
        printf("expression\n");
        destroyToken(token);
        return 0;
    }
    return parse_body(file,token, stack) || parse_main_body(file,token,stack);
}

// Function declartion
// <FUNC_DECLARE> -> [id]<PARAM> <FUNCTION_TYPE> <FUNC_BODY>
int parse_func_declare(FILE* file,Token* token, stack_t *stack){
    token = new_token(file,token);
    // [id] 
    if(token->type != identifier){
        destroyToken(token);
        return 2;
    }
    printf("ID\n");
    token = new_token(file,token);
    // <PARAM>
    if(parse_param(file,token)){
        destroyToken(token);
        return 2;
    }
    // <FUNCTION_TYPE>
    token = new_token(file,token);
    if(parse_function_type(file,token)){
        destroyToken(token);
        return 2;
    }
    // <FUNC_BODY>
    if(parse_function_body(file,token, stack)){
        destroyToken(token);
        return 2;
    }
    return 0;
}

// Parameters of the declared function
// <PARAM> -> (<PARAM_TYPES>) <NEXT_PARAM>
int parse_param(FILE*file, Token* token){
    // (
    if(strcmp(token->lexeme,"(")){
        destroyToken(token);
        printf("NOT \"(\" in func [id](<PARAM>) <FUNCTION_TYPE> {<FUNC_BODY>} <MAIN_BODY>");
        return 2;
    }
    printf("(\n");
    token = getToken(file);
    // <PARAM_TYPES> -> eps
    if(!strcmp(token->lexeme,")")){
        destroyToken(token);
        printf(")\n");
        return 0;
    }
    return parse_param_types(file,token) || parse_next_param(file,token);
}

// Type of the parameter of the declared function
//<PARAM_TYPES> -> [name] [id] : [type]
int parse_param_types(FILE* file, Token* token){
    // [name]
    if(token->type != identifier){
        printf("NOT \"ID\" in <PARAM_TYPES> -> [name] [id] : [type]");  
        destroyToken(token);
        return 2;
    }
    printf("NAME\n");
    token = new_token(file,token);
    // [id]
    if(token->type != identifier){
        printf("NOT \"ID\" in <PARAM_TYPES> -> [name] [id] : [type]");  
        destroyToken(token);
        return 2;
    }
    printf("ID\n");
    token = new_token(file,token);
    // :
    if(strcmp(token->lexeme,":")){
        printf("NOT \":\" in <PARAM_TYPES> -> [name] [id] : [type]");  
        destroyToken(token);
        return 2;
    }
    printf(":\n");
    token = new_token(file,token);
    // [type]
    if(token->type != variableType){
        destroyToken(token);
        return 2;
    }
    printf("TYPE\n");
    destroyToken(token);
    return 0;
}

// Next parameter of the declared function
// <NEXT_PARAM>
int parse_next_param(FILE* file, Token* token){
    token = getToken(file);
    // -> eps
    if(!strcmp(token->lexeme,")")){
            printf(")\n");
            destroyToken(token);
            return 0;
    }
    // -> , <PARAM_TYPES> <NEXT_PARAM>
    if(!strcmp(token->lexeme,",")){
            token = new_token(file,token);
            return parse_param_types(file,token) || parse_next_param(file,token);
    }
    destroyToken(token);
    return 2;
}

// Return type of the function
//<FUNCTION_TYPE> -> -> [type]
int parse_function_type(FILE *file, Token* token){
    // eps
    if(!strcmp(token->lexeme,"{")){
        printf("{\n");
        destroyToken(token);
        return 0;
    // ->
    }else if(!strcmp(token->lexeme,"-")){
        token = new_token(file,token);
        if(!strcmp(token->lexeme,">")){
            printf("->\n");
            token = new_token(file,token);
            printf("%s\n",token->lexeme);
            // [type]
            if(token->type == variableType){
                printf("TYPE\n");
                token = new_token(file,token);
                // {
                if(!strcmp(token->lexeme,"{")){
                    printf("{\n");
                    destroyToken(token);
                    return 0;
                }
            }
        }
    }
    destroyToken(token);
    return 2;
}

// Body of the declared function
// <FUNCTION_BODY>
int parse_function_body(FILE * file, Token * token, stack_t *stack){
    // -> eps
    token = getToken(file);
    if(!strcmp(token->lexeme,"}")){
            destroyToken(token);
            printf("}\n");
            return 0;
    }
    // -> return
    if(!strcmp(token->lexeme,"return")){
        token = new_token(file,token);
        return parse_expression(file,token, "TODO",stack) || parse_function_body(file,token,stack);
    }
    // if <IF_WHILE_EXPRESSION> <FUNC_BODY> <ELSE_FUNCTION_BODY> <FUNC_BODY>
    if(!strcmp(token->lexeme,"if")){
        printf("if\n");
        return parse_if_expression(file,token) || parse_function_body(file,token, stack) || parse_else_function_body(file,token,stack) || parse_function_body(file,token,stack);
    }
    // while <IF_WHILE_EXPRESSION> <IF_WHILE_MAIN_BODY>
    // TODO
    if(!strcmp(token->lexeme,"while")){
        printf("while\n");
        printf("expression\n");
        destroyToken(token);
        return 0;
    }
    // <BODY>
    if(parse_body(file,token,stack)){
        destroyToken(token);
        return 2;
    }
    return parse_function_body(file,token, stack);
}

// Body for function/main
// <BODY>
int parse_body(FILE* file, Token* token, stack_t *stack){
    // -> <DEF_VARIABLE> <ASSIGN>
    // <DEF_VARIABLE> -> let [id]
    // <DEF_VARIABLE> -> var [id]
    if(!strcmp(token->lexeme,"let") || !strcmp(token->lexeme,"var")){
        printf("%s\n",token->lexeme);
        token = new_token(file,token);
        if(token->type != identifier){
            destroyToken(token);
            return 2;
        }
        printf("%s\n",token->lexeme);
        return parse_assign(file,token,stack);
    }
    if(token->type == identifier){
        // -> [id](<CALL_PARAM>);
        printf("ID\n");
        token = new_token(file,token);
        if(!strcmp(token->lexeme,"(")){
            destroyToken(token);
            printf("(\n");
            return parse_call_param(file,token);
        }
        // -> [id] = <EXPRESSION>
        if(!strcmp(token->lexeme,"=")){
            printf("=\n");
            token = new_token(file,token);
            return parse_expression(file,token, "TODO", stack);
        }
    }
    destroyToken(token);
    return 2;
}

// Asign value
// <ASIGN>
int parse_assign(FILE* file, Token* token, stack_t *stack){
    char name[100];
    strcpy(name,token->lexeme);
    token = new_token(file,token);
    if(!strcmp(token->lexeme,"=")){
        // -> = [id](<CALL_PARAM>)
        printf("=\n");
        token = new_token(file,token);
        return parse_expression(file,token,name, stack);
    }
    // -> : [type] <VALUE>
    if(!strcmp(token->lexeme,":")){
        printf(":\n");
        token = new_token(file,token);
        if(token->type != variableType){
            destroyToken(token);
            return 2;
        }
        printf("TYPE\n");
        return 0;
        // TODO - ASSIGN VALUE
        /*
        token = new_token(file,token);
        if(!strcmp(token->lexeme,"=")){
            // = [id](<CALL_PARAM>)
            printf("=\n");
            token = new_token(file,token);
            return parse_expression(file,token);
        }
        */
    }
    destroyToken(token);
    return 2;
}

// Expression
// <EXPRESSION>
int parse_expression(FILE* file, Token* token, char* name, stack_t *stack){
    if(token->type == identifier){
        // -> [id](<CALL_PARAM>);
        printf("ID\n");
        token = new_token(file,token);
        if(!strcmp(token->lexeme,"(")){
            destroyToken(token);
            printf("(\n");
            return parse_call_param(file,token);
    }
    // -> [expression]
    printf("\n%s\n",name);
    printf("\nEXPRESSSIOOON\n");
    destroyToken(token);
    return 0;
}
addSymbol(token,name, stack);
// -> [expression]
printf("-> getting symbol...\n");
symbol_t *found = get_symbol(stack, name);
printf("Found variable with the name: %s\n", found->name);
printf("\nEXPRESSSIOOON\n");
destroyToken(token);
return 0;
}

// Parameters of the calling function
//<CALL_PARAM>
int parse_call_param(FILE * file, Token * token){
    //<CALL_PARAM> -> eps
    token = getToken(file);
    if(!strcmp(token->lexeme,")")){
            destroyToken(token);
            printf(")\n");
            return 0;
    }
    // -> <CALL_PARAM_TYPES>
    return parse_call_param_types(file,token);
}

// Types of parameters of the calling function
// <CALL_PARAM_TYPES>
int parse_call_param_types(FILE* file, Token* token){
    // -> [expression] <NEXT_CALL_PARAM>
    if(token->type == number || token->type == string || token->type == identifier){
        // -> name: expression <NEXT_CALL_PARAM>
        printf("%s\n",token->lexeme);
        token = new_token(file,token);
        if(!strcmp(token->lexeme,":")){
            printf(":\n");
            token = new_token(file,token);
            if(token->type == number || token->type == string || token->type == identifier){
                printf("%s\n",token->lexeme);
                token = new_token(file,token);
                // <NEXT_CALL_PARAM>
                return parse_next_call_param(file,token);
            }
            destroyToken(token);
            return 2;
        }
        // <NEXT_CALL_PARAM>
        return parse_next_call_param(file,token);
    }
    destroyToken(token);
    return 2;
}

// Next parameter of the calling function
// <NEXT_CALL_PARAM>
int parse_next_call_param(FILE* file, Token* token){
    //-> eps
    if(!strcmp(token->lexeme,")")){
            printf(")\n");
            destroyToken(token);
            return 0;
    }
    // -> , <CALL_PARAM_TYPES>
    if(!strcmp(token->lexeme,",")){
            token = new_token(file,token);
            return parse_call_param_types(file,token);
    }
    destroyToken(token);
    return 2;
}

// Condition in IF
// <IF_EXPRESSION>
int parse_if_expression(FILE* file, Token* token){
    token = new_token(file,token);
    // -> let [id] {
    if(!strcmp(token->lexeme,"let")){
        printf("let\n");
        token = new_token(file,token);
        if(token->type == identifier){
            printf("ID\n");
            token = new_token(file,token);
            //{
            if(!strcmp(token->lexeme,"{")){
                printf("{\n");
                destroyToken(token);
                //<FUNC_BODY>
                return 0;
            }
        }
        destroyToken(token);
        return 2;
    }
    // -> [expression]
    printf("EXPRESIOOON\n");
    destroyToken(token);
    return 0;
}

// Body in IF in MAIN
// <IF_WHILE_MAIN_BODY>
int parse_if_while_main_body(FILE* file,Token *token, stack_t *stack){
    token = getToken(file);
    // -> if <IF_EXPRESSION> <IF_WHILE_MAIN_BODY> <ELSE_MAIN_BODY> <IF_WHILE_MAIN_BODY>
    if(!strcmp(token->lexeme,"if")){
        printf("if\n");
        return parse_if_expression(file,token) || parse_if_while_main_body(file,token,stack) || parse_else_main_body(file,token,stack) || parse_if_while_main_body(file,token,stack);
    }
    // -> while [expression] <IF_WHILE_MAIN_BODY>
    if(!strcmp(token->lexeme,"while")){
        printf("while\n");
        printf("expression\n");
        destroyToken(token);
        return 0;
    }
    // eps
    if(!strcmp(token->lexeme,"}")){
        printf("}\n");
        destroyToken(token);
        return 0;
    }
    return parse_body(file,token, stack) || parse_if_while_main_body(file,token,stack);
}

// ELSE in MAIN
// <ELSE_MAIN_BODY>
int parse_else_main_body(FILE* file, Token* token, stack_t *stack){
    //-> else { <IF_WHILE_MAIN_BODY>
    token = getToken(file);
    // else
    if(!strcmp(token->lexeme,"else")){
        printf("else\n");
        token = new_token(file,token);
        // {
        if(!strcmp(token->lexeme,"{")){
            printf("{\n");
            destroyToken(token);
            // <IF_WHILE_MAIN_BODY>
            return parse_if_while_main_body(file,token,stack);
        }
        destroyToken(token);
        return 2;
    }
    destroyToken(token);
    return 2;
}

// ELSE in the body of function
// <ELSE_FUNCTION_BODY> -> else { <FUNC_BODY>
int parse_else_function_body(FILE* file, Token* token, stack_t *stack){
    token = getToken(file);
    // else
    if(!strcmp(token->lexeme,"else")){
        printf("else\n");
        token = new_token(file,token);
        //{
        if(!strcmp(token->lexeme,"{")){
            printf("{\n");
            destroyToken(token);
            //<FUNC_BODY>
            return parse_function_body(file,token,stack);
        }
        destroyToken(token);
        return 2;
    }
    destroyToken(token);
    return 2;
}
