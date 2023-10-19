#include "lexical.h"

Token handleIdentifier(FILE* file, char ch, int* col) {
    Token token = {.type = identifier, .lexeme = malloc(sizeof(char) * DEFAULT_LEXEME_LEN)};
    int lexlen = 0;
    token.lexeme[lexlen++] = ch;

    (*col)++;

    while (isalnum(ch = fgetc(file)) || ch == '_') {
        if (sizeof(token.lexeme) <= lexlen + 1) {
            token.lexeme = realloc(token.lexeme, sizeof(token.lexeme) + DEFAULT_LEXEME_LEN);
        }
        token.lexeme[lexlen++] = ch;
        (*col)++;
    }

    ungetc(ch, file);
    token.lexeme = realloc(token.lexeme, lexlen + 1);
    token.lexeme[lexlen] = '\0';
    return token;
}

Token handleNumber(FILE* file, char ch, int* col) {
    Token token = {.type = number, .lexeme = malloc(sizeof(char) * DEFAULT_LEXEME_LEN)};
    int lexlen = 0;
    token.lexeme[lexlen++] = ch;

    (*col)++;

    while (isdigit(ch = fgetc(file))) {
        if (sizeof(token.lexeme) <= lexlen + 1) {
            token.lexeme = realloc(token.lexeme, sizeof(token.lexeme) + DEFAULT_LEXEME_LEN);
        }
        token.lexeme[lexlen++] = ch;
        (*col)++;
    }

    ungetc(ch, file);
    token.lexeme = realloc(token.lexeme, lexlen + 1);
    token.lexeme[lexlen] = '\0';
    return token;
}

void handleComments(FILE* file, char ch) {
    // line comment
    if ((ch = getc(file)) == '/') {
        while ((ch = fgetc(file)) != '\n' && ch != EOF) {}
    }

    // multi-line comment
    else if (ch == '*') {
        while (((ch = fgetc(file)) != '*' &&
                (ch = fgetc(file)) != '/') ||
                ch != EOF) {
            ungetc(ch, file);
        }
    }

    // not a comment
    else {
        ungetc(ch, file);
    }
}

Token handleShit(FILE* file, char ch, int* col) {
    Token token = {.type = otherShit, .lexeme = malloc(sizeof(char) * 2)};

    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';

    (*col)++;

    return token;
}

Token handleOperator(FILE* file, char ch, int* col) {
    Token token = {.type = operation, .lexeme = malloc(sizeof(char) * 2)};

    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';

    (*col)++;

    // just solving these options "==", "!=", "<=", ">=", "??"
    if (((ch == '=' || ch == '!' || ch == '<' || ch == '>') && (ch = fgetc(file)) == '=') ||
         (token.lexeme[0] == '?' && ch == '?')) {
        token.lexeme = realloc(token.lexeme, sizeof(char) * 3);
        token.lexeme[1] = ch;
        token.lexeme[2] = '\0';
        (*col)++;
    }

    return token;
}

Token handleString(FILE* file, char ch, int* col) {
    Token token = {.type = string, .lexeme = malloc(sizeof(char) * DEFAULT_LEXEME_LEN)};
    int lexlen = 0;

    (*col)++;

    while ((ch = fgetc(file)) != '"' && ch != EOF) {
        if (sizeof(token.lexeme) <= lexlen + 1) {
            token.lexeme = realloc(token.lexeme, sizeof(token.lexeme) + DEFAULT_LEXEME_LEN);
        }
        token.lexeme[lexlen++] = ch;
        (*col)++;
    }

    (*col)++;

    token.lexeme = realloc(token.lexeme, lexlen + 1);
    token.lexeme[lexlen] = '\0';
    return token;
}

Token getToken(FILE* file) {
    if (!file) {
        Token token = {.lexeme = malloc(sizeof(char) * 2), .type = unknown};
        token.lexeme[0] = EOF;
        token.lexeme[1] = '\0';
        return token;
    }

    char ch;
    static int line = 0;
    static int col = 1;

    while ((ch = fgetc(file)) != EOF) {

        // whitespaces
        if (isspace(ch)) {
            if (ch == '\n') {
                col = 0;
                line++;
            }
            col++;
        }

        // comments
        else if (ch == '/') {
            handleComments(file, ch);
        }

        // keywords and identifiers
        else if (isalpha(ch) || ch == '_') {
            return handleIdentifier(file, ch, &col);
        }
        
        // numbers
        else if (isdigit(ch)) {
            return handleNumber(file, ch, &col);
        }
        
        // operators
        else if (ch == '=' ||
                 ch == '+' ||
                 ch == '-' ||
                 ch == '*' ||
                 ch == '/' ||
                 ch == '<' ||
                 ch == '>' ||
                 ch == '?' ) {
            return handleOperator(file, ch, &col);
        }

        // other shit
        else if (ch == ':' ||
                 ch == ',' ||
                 ch == '(' ||
                 ch == ')' ||
                 ch == '[' ||
                 ch == ']' ||
                 ch == '{' ||
                 ch == '}' ||
                 ch == '!') {
            return handleShit(file, ch, &col);
        }

        else if (ch == '"') {
            return handleString(file, ch, &col);
        }

        // unknown characters
        else {
            Token token = {.lexeme = malloc(sizeof(char) * 2), .type = unknown};
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            return token;
        }
    }

    Token token = {.lexeme = malloc(sizeof(char) * 2), .type = unknown};
    token.lexeme[0] = EOF;
    token.lexeme[1] = '\0';
    return token;
}
