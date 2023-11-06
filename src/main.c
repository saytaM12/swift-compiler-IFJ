#include "lexical.h"

void lexCheck() {
    Token* token;

    FILE* file = fopen("input.swift", "r");
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
        printf("%d,\t", token->type);
        printf("%s\n\n", token->lexeme);
        destroyToken(token);
        token = getToken(file);
    }

    free(token->lexeme);
    free(token);
    fclose(file);
}

int main(void) {
    putchar('\n');
    printf("===Printing Lexical analyzer output===");
    printf("\n\n");
    lexCheck();
    printf("\n\n");
}
