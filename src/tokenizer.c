#include "lexical.h"

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

void destroyToken(Token* token) {
    free(token->lexeme);
    free(token);
    token = NULL;
}
