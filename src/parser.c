#include "parser.h"
#include "generator.h"
#include "lexical.h"
#include "symstack.h"
#include "symtable.h"
#include "expression.h"

code_t code;
instruction_t *ins;

Typee *param_types = NULL;
int size = 0;
int size_call_function = 0;

struct CALLFUNCTION{
    char name[100];
    int size;
    Typee *param_types;
};

struct CALLFUNCTION *call_function;

code_t code;
instruction_t *ins;

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
                if(!strcmp(token->lexeme,"Double")){
                    symbol = symbol_variable_ctor(name, double_tt);
                    add_symbol(stack, symbol);
                }
                break;
            case number:
                symbol = symbol_variable_ctor(name, int_t);
                add_symbol(stack, symbol);
                break;
            case numberFloat:
                symbol = symbol_variable_ctor(name, double_tt);
                add_symbol(stack, symbol);
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
// Vestavene funkce
void addFunctions(stack_t *stack){
    symbol_t *symbol = symbol_function_ctor("readString", string_t , NULL , 0);
    add_symbol(stack, symbol);
    symbol = symbol_function_ctor("readInt", int_t , NULL , 0);
    add_symbol(stack, symbol);
    symbol = symbol_function_ctor("readDouble", int_t , NULL , 0);
    add_symbol(stack, symbol);
    symbol = symbol_function_ctor("write", undefined_t , NULL , -1);
    add_symbol(stack, symbol);
    Typee param_types[1] ={int_t};
    symbol = symbol_function_ctor("Int2Double", double_tt , param_types , 1);
    add_symbol(stack, symbol);
    Typee param_types2[1] ={double_tt};
    symbol = symbol_function_ctor("Double2Int", int_t , param_types2 , 1);
    add_symbol(stack, symbol);
    Typee param_types3[1] ={string_t};
    symbol = symbol_function_ctor("length", int_t , param_types3 , 1);
    add_symbol(stack, symbol);
    Typee param_types4[3] ={string_t,int_t,int_t};
    symbol = symbol_function_ctor("substring", string_t , param_types4 , 3);
    add_symbol(stack, symbol);
    Typee param_types5[1] ={string_t};
    symbol = symbol_function_ctor("ord", int_t , param_types5 , 1);
    add_symbol(stack, symbol);
    Typee param_types6[1] ={int_t};
    symbol = symbol_function_ctor("chr", string_t , param_types6 , 1);
    add_symbol(stack, symbol);
    return;
}

// Start of the program
int parse_prog(){
    code = generator_code_init();
    ins = generator_ins_init();
    FILE* file = fopen("input2.swift", "r");
    Token* token = NULL;
    stack_t *stack = stack_ctor();
    push_new_scope(stack);
    addFunctions(stack);
    return parse_main_body(file,token, stack);
}

// Main body
// <MAIN_BODY> 
int parse_main_body(FILE *file, Token* token, stack_t *stack){
    token = getToken(file);
    // -> eps
    if(token->lexeme[0] == EOF){
        // Kontrola volanych funkci
        for(int i = 0; i <size_call_function;i++){
            // Jestli je funkce definovana
            symbol_t *found = get_symbol(stack, call_function[i].name);
                if(found == NULL){
                    printf("Nedefinovana funkce %s\n",call_function[i].name);
                    return 3;
                }

                // Parametry
            // FIX THIS --- spatna velikost pole ,sizeof(call_function[i].param_types)/sizeof(Typee)
            printf("pole %d hash %d\n",call_function[i].size,found->size);
            // Kvuli funkci write, ktera muze mit parametru kolik chce
            if(found->size != -1){
            if(call_function[i].size != found->size){
                printf("spatny pocet argumentu");
                return 4;
            }
            }
            for(int j = 0;j < call_function[i].size;j++){
                // Kvuli funkci write, ktera muze mit parametru kolik chce
                if(found->size != -1){
                if(found->param_types[j] != call_function[i].param_types[j]){
                    printf("spatny typ");
                    printf("typ %d typ %d",found->param_types[j], call_function[i].param_types[j]);
                    return 4;
                }
                }
            }
        }
        pop_scope(stack);
        stack_dtor(stack);
        destroyToken(token);
        return 0;
    }
    // -> func <FUNCTION_DECLARE>
    if(!strcmp(token->lexeme, "func")){
        printf("\nfunc\n");

        ins->instructionType = funDef;
        ins->funDef.paramNum = 0;
        ins->funDef.parameters = calloc(sizeof(struct funDefParam *), 1);

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
        ERROR();
        return 2;
    }
    printf("%s\n",token->lexeme);
    char name[100];
    strcpy(name,token->lexeme);
    ins->funDef.name = calloc(sizeof(token->lexeme), 1);
    strcpy(ins->funDef.name, token->lexeme);
    token = new_token(file,token);
    // <PARAM>
    if(parse_param(file,token,stack)){
        ERROR();
        return 2;
    }
    // <FUNCTION_TYPE>
    token = new_token(file,token);
    if(parse_function_type(file,token,name,stack)){
        ERROR();
        return 2;
    }
    push_new_scope(stack);
    // <FUNC_BODY>
    if(parse_function_body(file,token, stack)){
        ERROR();
        return 2;
    }
    return 0;
}

// Parameters of the declared function
// <PARAM> -> (<PARAM_TYPES>) <NEXT_PARAM>
int parse_param(FILE*file, Token* token,stack_t* stack){
    // (
    if(strcmp(token->lexeme,"(")){
        ERROR();
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
    return parse_param_types(file,token,stack) || parse_next_param(file,token,stack);
}

// Type of the parameter of the declared function
//<PARAM_TYPES> -> [name] [id] : [type]
int parse_param_types(FILE* file, Token* token, stack_t* stack){
    // [name]
    if(token->type != identifier){
        printf("NOT \"ID\" in <PARAM_TYPES> -> [name] [id] : [type]");  
        ERROR();
        return 2;
    }
    printf("NAME\n");

    struct funDefParam *newParam = calloc(sizeof(struct funDefParam), 1);
    newParam->name = calloc(sizeof(token->lexeme), 1);
    strcpy(newParam->name, token->lexeme);

    token = new_token(file,token);
    // [id]
    if(token->type != identifier){
        printf("NOT \"ID\" in <PARAM_TYPES> -> [name] [id] : [type]");  
        free(newParam);
        ERROR();
        return 2;
    }
    char name[100];
    strcpy(name,token->lexeme);
    printf("ID\n");

    newParam->id = calloc(sizeof(token->lexeme), 1);
    strcpy(newParam->id, token->lexeme);

    token = new_token(file,token);
    // :
    if(strcmp(token->lexeme,":")){
        printf("NOT \":\" in <PARAM_TYPES> -> [name] [id] : [type]");  
        free(newParam);
        ERROR();
        return 2;
    }
    printf(":\n");
    token = new_token(file,token);
    // [type]
    if(token->type != variableType){
        ERROR();
        return 2;
    }
    printf("TYPE\n");
    newParam->type = ENUMTYPE(token);
    ins->funDef.parameters = realloc(ins->funDef.parameters, sizeof(struct funDefParam *) * (ins->funDef.paramNum + 1));
    ins->funDef.parameters[ins->funDef.paramNum++] = newParam;
    size++;
    param_types = realloc(param_types, size * sizeof(Typee));
    if (!strcmp(token->lexeme,"Int")){
        param_types[size-1] = int_t;
    }if(!strcmp(token->lexeme,"String")){
        param_types[size-1] = string_t;
    }
    if(!strcmp(token->lexeme,"Double")){
        param_types[size-1] = double_tt;
    }
    destroyToken(token);
    return 0;
}

// Next parameter of the declared function
// <NEXT_PARAM>
int parse_next_param(FILE* file, Token* token,stack_t* stack){
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
        return parse_param_types(file,token,stack) || parse_next_param(file,token,stack);
    }
    ERROR();
    return 2;
}

// Return type of the function
//<FUNCTION_TYPE> -> -> [type]
int parse_function_type(FILE *file, Token* token, char* name, stack_t* stack){
    // eps
    printf("\n%s\n",token->lexeme);
    if(!strcmp(token->lexeme,"{")){
        symbol_t *found = get_symbol(stack, name);
                    if(found!=NULL){
                        return 3;
                    }
                    symbol_t *symbol = symbol_function_ctor(name, void_t, param_types,size);
                    add_symbol(stack, symbol);
                    size=0;
        printf("{\n");
        destroyToken(token);
        generator_translate();
        return 0;
    // ->
    }else if(!strcmp(token->lexeme,"->")){
            printf("->\n");
            token = new_token(file,token);
            printf("%s\n",token->lexeme);
            // [type]
            if(token->type == variableType){
                printf("TYPE\n");
                ins->funDef.type = ENUMTYPE(token);
                int typ;
                if (!strcmp(token->lexeme,"Int")){
                    typ = int_t;
                }else if(!strcmp(token->lexeme,"String")){
                    typ = string_t;
                }
                else if(!strcmp(token->lexeme,"Double"))
                    typ = double_tt;
                else{
                    typ = -1;
                }
                // Redefinice funkce
                symbol_t *found = get_symbol(stack, name);
                if(found!=NULL){
                    return 3;
                }
                symbol_t *symbol = symbol_function_ctor(name, typ, param_types,size);
                add_symbol(stack, symbol);
                size=0;
                token = new_token(file,token);
                // {
                if(!strcmp(token->lexeme,"{")){
                    printf("{\n");
                    destroyToken(token);
                    generator_translate();
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
        ERROR();
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
            ERROR();
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

        char *potentialFun = malloc(strlen(token->lexeme) + 1);
        strcpy(potentialFun, token->lexeme);

        token = new_token(file,token);
        if(!strcmp(token->lexeme,"(")){

            ins->instructionType = funCal;
            ins->funCal.name = potentialFun;
            ins->funCal.paramNum = 0;
            ins->funCal.parameters = calloc(sizeof(struct funDefParam *), 1);

            destroyToken(token);
            printf("(\n");
            return parse_call_param(file,token,name,stack);
        }

        // -> [id] = <EXPRESSION>
        if(!strcmp(token->lexeme,"=")){
            symbol_t *found = get_symbol(stack, name);
            if(found == NULL){
                printf("not defined");
                return 3;
            }
            printf("=\n");
            free(potentialFun);
            token = new_token(file,token);
            char name[100];
            return parse_expression(file,token, name, stack);
        }
    }
    ERROR();
    return 2;
}

// Asign value
// <ASIGN>
int parse_assign(FILE* file, Token* token, char*name,stack_t *stack){
    token = new_token(file,token);
    if(!strcmp(token->lexeme,"=")){
        // -> = [id](<CALL_PARAM>)
        printf("= \n");
        token = new_token(file,token);
        return parse_expression(file,token,name, stack);
    }
    // -> : [type] <VALUE>
    if(!strcmp(token->lexeme,":")){
        printf(":\n");
        token = new_token(file,token);
        if(token->type != variableType){
            ERROR();
            return 2;
        }
        if(addSymbol(token,name, stack))
            return addSymbol(token,name, stack);
        printf("TYPE\n");
        token = new_token(file,token);
        if(!strcmp(token->lexeme,"?")){
            token = new_token(file,token);
        }
        if(!strcmp(token->lexeme,"=")){
            printf("=\n");
            token = new_token(file,token);
            return parse_expression(file,token,"",stack);
        }
    }
    ERROR();
    return 2;
}

// Expression
// <EXPRESSION>
int parse_expression(FILE* file, Token* token, char* name, stack_t *stack){
    if(token->type == identifier){
        //symbol_t *found = get_symbol(stack, name);
        // -> [id](<CALL_PARAM>);
        printf("IDee\n");
        char namee[100];
        strcpy(namee,token->lexeme);
        token = new_token(file,token);
        if(!strcmp(token->lexeme,"(")){
            destroyToken(token);
            printf("(\n");
            printf("%s",namee);
            return parse_call_param(file,token,namee, stack);
        returnToken(token, file);
    }
    // -> [expression]
    printf("\nEXPRESSSIOOON\n");
    expression_value *value=bottomUp(token,file);
    if (value==NULL)
    {
        return 2;
    }

    printValue(value,0);
    disposeValue(value);
    destroyToken(token);
    return 0;
}

printf("\nEXPRESSSIOOON\n");
    expression_value *value=bottomUp(token,file);
    if (value==NULL)
    {
        return 2;
    }
    printValue(value,0);
    disposeValue(value);
    // for (int i = 0; i < token->lexlen-1; i++)
    // {
    //     printf("%c\n",token->lexeme[i]);
    // }
    // printf("%s\n",token->lexeme);
// destroyToken(token);
return 0;
}

// Parameters of the calling function
//<CALL_PARAM>
int parse_call_param(FILE * file, Token * token, char *name, stack_t* stack){
    size_call_function++;
    size++;
    call_function = realloc(call_function, size_call_function * sizeof(struct CALLFUNCTION));
    strcpy(call_function[size_call_function-1].name,name);
    //<CALL_PARAM> -> eps
    token = getToken(file);
    if(!strcmp(token->lexeme,")")){
        call_function[size_call_function-1].param_types = NULL;
        call_function[size_call_function-1].size = 0;
        size = 0;
        destroyToken(token);
        printf(")\n");
        return 0;
    }
    // -> <CALL_PARAM_TYPES>
    return parse_call_param_types(file,token,name,stack);
}

// Types of parameters of the calling function
// <CALL_PARAM_TYPES>
int parse_call_param_types(FILE* file, Token* token, char *name, stack_t* stack){
    // -> [expression] <NEXT_CALL_PARAM>
    if(token->type == number || token->type == string || token->type == identifier || token->type == numberFloat){
         if(size == 1)
            call_function[size_call_function-1].param_types = malloc(size * sizeof(Typee));
         else
            call_function[size_call_function-1].param_types = realloc(call_function[size_call_function-1].param_types,size * sizeof(Typee));
        // -> name: expression <NEXT_CALL_PARAM>
        printf("%s\n",token->lexeme);

        ins->funCal.parameters[ins->funCal.paramNum] = malloc(strlen(token->lexeme) + 1);
        strcpy(ins->funCal.parameters[ins->funCal.paramNum++], token->lexeme);

        int typ = token->type;
        token = new_token(file,token);
        if(!strcmp(token->lexeme,":")){
            printf(":\n");
            token = new_token(file,token);
            if(token->type == number || token->type == string || token->type == identifier || token->type == numberFloat){
                if(token->type == number){
                    call_function[size_call_function-1].param_types[size-1] = int_t;
                }
                if(token->type == string){
                    call_function[size_call_function-1].param_types[size-1] = string_t;
                }
                if(token->type == numberFloat){
                    call_function[size_call_function-1].param_types[size-1] = double_tt;
                }
                if(token->type == identifier){
                    symbol_t *found = get_symbol(stack, token->lexeme);
                    if(found == NULL){
                        return 3;
                    }
                    call_function[size_call_function-1].param_types[size-1] = found->type;
                }
                printf("%s\n",token->lexeme);

                ins->funCal.parameters[ins->funCal.paramNum - 1] =
                    realloc(ins->funCal.parameters[ins->funCal.paramNum - 1], strlen(token->lexeme) + 1);
                strcpy(ins->funCal.parameters[ins->funCal.paramNum - 1], token->lexeme);

                token = new_token(file,token);
                // <NEXT_CALL_PARAM>
                return parse_next_call_param(file,token,name,stack);
            }
            ERROR();
            return 2;
        }
        printf("size_fnc:%d size:%d\n",size_call_function,size);
        if(typ == number)
            call_function[size_call_function-1].param_types[size-1] = int_t;
        if(typ == string)
            call_function[size_call_function-1].param_types[size-1] = string_t;
        if(typ == numberFloat)
            call_function[size_call_function-1].param_types[size-1] = double_tt;
        if(typ == identifier){
            symbol_t *found = get_symbol(stack, name);
            if(found == NULL){
                return 3;
            }
            call_function[size_call_function-1].param_types[size-1] = found->type;
        }
        // <NEXT_CALL_PARAM>
        return parse_next_call_param(file,token,name, stack);
    }
    ERROR();
    return 2;
}

// Next parameter of the calling function
// <NEXT_CALL_PARAM>
int parse_next_call_param(FILE* file, Token* token, char *name, stack_t* stack){
    //-> eps
    if(!strcmp(token->lexeme,")")){
            printf(")\n");
            call_function[size_call_function-1].size = size;
            size = 0;
            destroyToken(token);
            return 0;
    }
    // -> , <CALL_PARAM_TYPES>
    if(!strcmp(token->lexeme,",")){
            token = new_token(file,token);
            size++;
            return parse_call_param_types(file,token,name, stack);
    }
    ERROR();
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
        ERROR();
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
        ERROR();
        return 2;
    }
    ERROR();
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
        ERROR();
        return 2;
    }
    ERROR();
    return 2;
}
