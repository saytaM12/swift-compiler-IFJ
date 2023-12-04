#include "lexical.h"
#include "parser.h"
#include "expression.h"
#include "generator.h"

void lexCheck() {
    Token* token;

    FILE* file = fopen("input.swift", "r");
    if (!file) {
        return;
    }
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

void genCheck() {
    FILE *file = fopen("code.txt", "w");

    generator_addLineEnd(&code, "line1");
    generator_addLineEnd(&code, "line2");
    generator_addLineEnd(&code, "line3");
    generator_addLineEnd(&code, "line4");
    generator_addLineFromEnd(&code, "line3.5", 1);

    generator_write(file, code);
    generator_destroy(&code);
}

int main(void) {
    /*
    printf("\n===Printing Lexical analyzer output===\n\n");
    lexCheck();
    pos.line = 1;
    pos.col = 0;
    */
    printf("\n\n====Printing top-down parser output===\n\n");
    synCheck();
    printf("\n\n====Printing generator output to a file===");
    genCheck();
    printf("\n\n");
}
