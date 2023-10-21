#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    // Add more token types as needed
    TOKEN_UNKNOWN
} TokenType;

// Define a structure to represent a token
typedef struct {
    TokenType type;
    char* lexeme;
} Token;

// Function to initialize a token
void createToken(Token *token) {
    token->lexeme = malloc(sizeof(char) * 100);
}

// Function to free memory used by a token
void freeToken(Token *token) {
    free(token->lexeme);
    free(token);
}

// Function to tokenize the input Swift code
void getNextToken(FILE* input, Token *token) {
    int bufferIndex = 0;
    char ch;

    // Read characters from the input file and build the token
    while ((ch = fgetc(input)) != EOF) {
        if (isalpha(ch) || ch == '_') {
            // Handle identifiers and keywords
            token->lexeme[bufferIndex++] = ch;
            while (isalnum(ch = fgetc(input)) || ch == '_') {
                token->lexeme[bufferIndex++] = ch;
            }
            ungetc(ch, input);
            token->lexeme[bufferIndex] = '\0';
            token->type = TOKEN_IDENTIFIER;
            return;
        } else if (isdigit(ch)) {
            // Handle numbers
            token->lexeme[bufferIndex++] = ch;
            while (isdigit(ch = fgetc(input))) {
                token->lexeme[bufferIndex++] = ch;
            }
            ungetc(ch, input);
            token->lexeme[bufferIndex] = '\0';
            token->type = TOKEN_NUMBER;
            return;
        }
        // Add more cases to handle keywords, strings, operators, and punctuation

        // Handle whitespace and comments
        else if (isspace(ch)) {
            continue;
        } else if (ch == '/') {
            ch = fgetc(input);
            if (ch == '/') {
                while ((ch = fgetc(input)) != '\n' && ch != EOF);
                continue;
            } else {
                ungetc(ch, input);
                token->type = TOKEN_OPERATOR;
                token->lexeme = "/";
                return;
            }
        } else {
            // Handle unknown characters
            token->lexeme[0] = ch;
            token->lexeme[1] = '\0';
            token->type = TOKEN_UNKNOWN;
            return;
        }
    }

    token->lexeme[0] = 'E';
    token->lexeme[1] = 'O';
    token->lexeme[2] = 'F';
    token->lexeme[3] = '\0';
    token->type = TOKEN_UNKNOWN;
}

int main() {
    FILE* inputFile = fopen("input.swift", "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    Token *token = malloc(sizeof(Token));
    createToken(token);
    do {
        getNextToken(inputFile, token);
        // Process the token (e.g., print it)
        printf("Token Type: %d, Lexeme: %s\n", token->type, token->lexeme);
    } while (token->type != TOKEN_UNKNOWN || strcmp(token->lexeme, "EOF") != 0);

    freeToken(token);

    fclose(inputFile);
    return 0;
}

