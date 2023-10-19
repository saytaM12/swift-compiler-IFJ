#include "lexical.h"

int main(void) {
    Token token;

    FILE* file = fopen("input.swift", "r");
    token = getToken(file);

    while (token.type != unknown) {
        printf("%d,\t", token.type);
        printf("%s\n", token.lexeme);
        free(token.lexeme);
        token = getToken(file);
    }

    free(token.lexeme);
    fclose(file);
}
