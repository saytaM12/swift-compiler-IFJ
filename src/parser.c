#include "parser.h"
#include "symstack.h"
#include "symtable.h"


Typee *param_types = NULL;
int size = 0;
int size_call_function = 0;
struct CALLFUNCTION{
    char name[1000];
    Typee *param_types;
};

struct CALLFUNCTION *call_function;
// Get new and delete old token
Token* new_token(FILE* file, Token* token){
    destroyToken(token);
    return getToken(file);
}

int addSymbol(Token* token, char* name, stack_t *stack){
    symbol_t *symbol;
    symbol_t *found = get_symbol_top(stack, name);
    // Redefinice promenne
        if(found!=NULL){
            return 3;
        }
    switch (token->type) {
            case identifier:
                printf("(identifier)   ");
            case keyword:
                return 9;
            case variableType:
                if (!strcmp(token->lexeme,"Int"))
                {
                    symbol = symbol_variable_ctor(name, int_t);
                    add_symbol(stack, symbol);
                }if(!strcmp(token->lexeme,"String")){
                    symbol = symbol_variable_ctor(name, string_t);
                    add_symbol(stack, symbol);
                }
                break;
            case number:
                symbol = symbol_variable_ctor(name, int_t);
                add_symbol(stack, symbol);
                break;
            case operation:
                return 9;
            case singleChars:
                return 9;
                break;
            case string:
                symbol = symbol_variable_ctor(name, string_t);
                add_symbol(stack, symbol);
                break;
            case unknown:
                return 9;
        }
    return 0;
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
        /*
        for(int i = 0; i <size_call_function;i++){
            printf("----ted-----");
            printf("%s\n",call_function[i].name);
            printf("typp: %d\n",call_function[i].param_types[0]);
        }
        */
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
        return parse_if_expression(file,token,stack) || parse_if_while_main_body(file,token,stack) || parse_else_main_body(file,token,stack) || parse_main_body(file,token, stack);
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
        return 2;
    }
    printf("%s\n",token->lexeme);
    char name[100];
    strcpy(name,token->lexeme);
    token = new_token(file,token);
    // <PARAM>
    if(parse_param(file,token)){
        return 2;
    }
    // <FUNCTION_TYPE>
    token = new_token(file,token);
    if(parse_function_type(file,token,name,stack)){
        return 2;
    }
    push_new_scope(stack);
    // <FUNC_BODY>
    if(parse_function_body(file,token, stack)){
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
    size++;
    param_types = realloc(param_types, size * sizeof(Typee));
    if (!strcmp(token->lexeme,"Int")){
        param_types[size-1] = int_t;
    }if(!strcmp(token->lexeme,"String")){
        param_types[size-1] = string_t;
    }
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
int parse_function_type(FILE *file, Token* token, char* name, stack_t* stack){
    // eps
    printf("\n%s\n",token->lexeme);
    if(!strcmp(token->lexeme,"{")){
        printf("{\n");
        destroyToken(token);
        return 0;
    // ->
    }else if(!strcmp(token->lexeme,"->")){
            printf("->\n");
            token = new_token(file,token);
            printf("%s\n",token->lexeme);
            // [type]
            if(token->type == variableType){
                printf("TYPE\n");
                int typ;
                if (!strcmp(token->lexeme,"Int")){
                    typ = int_t;
                }if(!strcmp(token->lexeme,"String")){
                    typ = string_t;
                }
                // Redefinice funkce
                symbol_t *found = get_symbol(stack, name);
                if(found!=NULL){
                    return 3;
                }
                symbol_t *symbol = symbol_function_ctor(name, typ, param_types);
                add_symbol(stack, symbol);
                size=0;
                token = new_token(file,token);
                // {
                if(!strcmp(token->lexeme,"{")){
                    printf("{\n");
                    destroyToken(token);
                    return 0;
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
            pop_scope(stack);
            destroyToken(token);
            printf("}\n");
            return 0;
    }
    // -> return
    if(!strcmp(token->lexeme,"return")){
        printf("return\n");
        token = new_token(file,token);
        return parse_expression(file,token, NULL,stack) || parse_function_body(file,token,stack);
    }
    // if <IF_WHILE_EXPRESSION> <FUNC_BODY> <ELSE_FUNCTION_BODY> <FUNC_BODY>
    if(!strcmp(token->lexeme,"if")){
        printf("if\n");
        return parse_if_expression(file,token,stack) || parse_function_body(file,token, stack) || parse_else_function_body(file,token,stack) || parse_function_body(file,token,stack);
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
        char name[100];
        strcpy(name,token->lexeme);
        return parse_assign(file,token,name,stack);
    }
    if(token->type == identifier){
        // -> [id](<CALL_PARAM>);
        printf("ID\n");
        char name[100];
        strcpy(name,token->lexeme);
        token = new_token(file,token);
        if(!strcmp(token->lexeme,"(")){
            destroyToken(token);
            printf("(\n");
            return parse_call_param(file,token,name);
        }

        // -> [id] = <EXPRESSION>
        if(!strcmp(token->lexeme,"=")){
            printf("=\n");
            token = new_token(file,token);
            char name[100];
            return parse_expression(file,token, name, stack);
        }
    }
    destroyToken(token);
    return 2;
}

// Asign value
// <ASIGN>
int parse_assign(FILE* file, Token* token, char*name,stack_t *stack){
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
        if(addSymbol(token,name, stack))
            return addSymbol(token,name, stack);
        printf("TYPE\n");
        token = new_token(file,token);
         printf("-> getting symbol...\n");
        symbol_t *found = get_symbol(stack, name);
        printf("Found variable with the name: %s typ: %d\n", found->name,found->type);
        if(!strcmp(token->lexeme,"=")){
            // = [id](<CALL_PARAM>)
            printf("=\n");
            token = new_token(file,token);
            return parse_expression(file,token,name,stack);
        }
        return 0;

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
        strcpy(name,token->lexeme);
        token = new_token(file,token);
        if(!strcmp(token->lexeme,"(")){
            destroyToken(token);
            printf("(\n");
            return parse_call_param(file,token,name);
    }
    // -> [expression]
    printf("\n%s\n",name);
    printf("\nEXPRESSSIOOON\n");
    destroyToken(token);
    return 0;
}
if(name != NULL){
    if(addSymbol(token,name, stack))
        return addSymbol(token,name, stack);
    // -> [expression]
    printf("-> getting symbol...\n");
    symbol_t *found = get_symbol(stack, name);
    printf("Found variable with the name: %s typ: %d\n", found->name,found->type);
}
printf("\nEXPRESSSIOOON\n");
destroyToken(token);
return 0;
}

// Parameters of the calling function
//<CALL_PARAM>
int parse_call_param(FILE * file, Token * token, char *name){
    size_call_function++;
    call_function = realloc(call_function, size_call_function * sizeof(struct CALLFUNCTION));
    strcpy(call_function[size_call_function-1].name,name);
    //<CALL_PARAM> -> eps
    token = getToken(file);
    if(!strcmp(token->lexeme,")")){
        call_function[size_call_function-1].param_types = NULL;
        size_call_function = 0;
        size = 0;
        destroyToken(token);
        printf(")\n");
        return 0;
    }
    // -> <CALL_PARAM_TYPES>
    return parse_call_param_types(file,token,name);
}

// Types of parameters of the calling function
// <CALL_PARAM_TYPES>
int parse_call_param_types(FILE* file, Token* token, char *name){
    // -> [expression] <NEXT_CALL_PARAM>
    if(token->type == number || token->type == string || token->type == identifier){
        // -> name: expression <NEXT_CALL_PARAM>
        printf("%s\n",token->lexeme);
        token = new_token(file,token);
        if(!strcmp(token->lexeme,":")){
            printf(":\n");
            token = new_token(file,token);
            if(token->type == number || token->type == string || token->type == identifier){
                size++;
                call_function[size_call_function-1].param_types = realloc(call_function[size_call_function-1].param_types, size * sizeof(Typee));
                if(token->type == number){
                    call_function[size_call_function-1].param_types[size-1] = int_t;
                }
                if(token->type == string)
                    call_function[size_call_function-1].param_types[size-1] = string;
                printf("%s\n",token->lexeme);
                token = new_token(file,token);
                // <NEXT_CALL_PARAM>
                return parse_next_call_param(file,token,name);
            }
            destroyToken(token);
            return 2;
        }
        // <NEXT_CALL_PARAM>
        return parse_next_call_param(file,token,name);
    }
    destroyToken(token);
    return 2;
}

// Next parameter of the calling function
// <NEXT_CALL_PARAM>
int parse_next_call_param(FILE* file, Token* token, char *name){
    //-> eps
    if(!strcmp(token->lexeme,")")){
            printf(")\n");
            destroyToken(token);
            return 0;
    }
    // -> , <CALL_PARAM_TYPES>
    if(!strcmp(token->lexeme,",")){
            token = new_token(file,token);
            return parse_call_param_types(file,token,name);
    }
    destroyToken(token);
    return 2;
}

// Condition in IF
// <IF_EXPRESSION>
int parse_if_expression(FILE* file, Token* token, stack_t *stack){
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
                push_new_scope(stack);
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
        return parse_if_expression(file,token,stack) || parse_if_while_main_body(file,token,stack) || parse_else_main_body(file,token,stack) || parse_if_while_main_body(file,token,stack);
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
        pop_scope(stack);
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
            push_new_scope(stack);
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
            push_new_scope(stack);
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
