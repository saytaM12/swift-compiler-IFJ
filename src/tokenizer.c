#include "tokenizer.h"
#include <stdio.h>

Token* initToken() {
    Token* token = malloc(sizeof(Token));
    token->lexeme = malloc(sizeof(char) * DEFAULT_LEXEME_LEN);
    token->size = DEFAULT_LEXEME_LEN;
    token->lexlen = 0;
    token->type = unknown;
    return token;
}

void addToLexeme(Token* token, char ch) {
    if (token->size <= token->lexlen + 1) {
        token->lexeme = realloc(token->lexeme, token->size + DEFAULT_LEXEME_LEN);
        token->size += DEFAULT_LEXEME_LEN;
    }
    token->lexeme[token->lexlen++] = ch;
}

void finishToken(Token* token, Type type) {
    token->lexeme = realloc(token->lexeme, token->lexlen + 1);
    token->size = token->lexlen + 1;
    token->lexeme[token->lexlen] = '\0';
    token->type = type;
}

void returnToken(Token *token, FILE *file) {
    for (int i = 0; i < token->lexlen; i++) {
        ungetc(token->lexeme[token->lexlen - (i + 1)], file);
    }
}

void destroyToken(Token* token) {
    if (!token) {
        return;
    }
    if (token->lexeme) {
        free(token->lexeme);
    }
    token->lexeme = NULL;
    free(token);
    token = NULL;
}

void changeTokenType(Token* token, Type type) {
    token->type = type;
}
