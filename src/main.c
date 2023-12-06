#include "lexical.h"
#include "parser.h"
#include "generator.h"
#include "expression.h"
#include "symstack.h"
#include "tokenizer.h"

void lexCheck() {
    Token* token;

    int i = 0;
    FILE* file = fopen("input.swift", "r");
    if (!file) return;
    token = getToken(file);

    while (token->lexeme[0] != EOF) {
        switch (token->type) {
            case identifier:
                printf("(identifier)   ");
                break;
            case keyword:
                printf("(keyword)      ");
                break;
            case variableType:
                printf("(variableType) ");
                break;
            case number:
                printf("(number)       ");
                break;
            case numberFloat:
                printf("(numberFloat)  ");
                break;
            case operation:
                printf("(operation)    ");
                break;
            case singleChars:
                printf("(singleChars)  ");
                break;
            case string:
                printf("(string)       ");
                break;
            case unknown:
                printf("(unknown)      ");
                break;
        }
        printf("%d:%d\t", pos.line, pos.col);
        printf("%s\n", token->lexeme);
        if (i < 20) {
            returnToken(token, file);
            i++;
        }
        destroyToken(token);
        token = getToken(file);
    }

    free(token->lexeme);
    free(token);
    fclose(file);
}

void synCheck(){
    if(!parse_prog()) {
        printf("gud");
    } else {
        printf("BAAAD");
    }
}

int main(void) {
    printf("\n===Printing Lexical analyzer output===\n\n");
    lexCheck();
    pos.line = 1;
    pos.col = 0;
    printf("\n\n====Printing top-down parser output===\n\n");
    synCheck();
    printf("\n\n====Printing generator output to a file===");
    FILE *file = fopen("code.txt", "w");
    generator_write(file, code);
    fclose(file);
    generator_code_destroy(&code);
    printf("\n\n");
}
