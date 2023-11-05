#include "lexical.h"

const char keywords[NUMOFKEYWORDS][20] = {"Double", "else", "func", "if", "Int", "let", "nil", "return", "String", "var", "while"};

Token* handleIdentifier(FILE* file, char ch, FilePos* pos) {
    Token* token = initToken();
    addToLexeme(token, ch);

    pos->col++;

    while (isalnum(ch = fgetc(file)) || ch == '_') {
        addToLexeme(token, ch);
        pos->col++;
    }

    ungetc(ch, file);
    finishToken(token, identifier);

    // check for keywords
    for (int i = 0; i < NUMOFKEYWORDS; i++) {
        if (strcmp(token->lexeme, keywords[i]) == 0) {
            changeTokenType(token, keyword);
            break;
        }
    }

    return token;
}

Token* handleNumber(FILE* file, char ch, FilePos* pos) {
    Token* token = initToken();
    addToLexeme(token, ch);

    pos->col++;

    while (isdigit(ch = fgetc(file))) {
        addToLexeme(token, ch);
        pos->col++;
    }

    ungetc(ch, file);
    finishToken(token, number);
    return token;
}

void handleComments(FILE* file, char ch) {
    // line comment
    if ((ch = getc(file)) == '/') {
        while ((ch = fgetc(file)) != '\n' && ch != EOF) {}
    }

    // multi-line comment
    else if (ch == '*') {
        int check[2];
        while ((ch = fgetc(file)) != EOF) {
            if ((check[0] = fgetc(file)) == '*') {
                if ((check[1] = fgetc(file)) == '/') {
                    return;
                } else { ungetc(check[1], file); }
            } else { ungetc(check[0], file); }
        }
    }

    // not a comment
    else {
        ungetc(ch, file);
    }
}

Token* handleSingleChars(FILE* file, char ch, FilePos* pos) {
    Token* token = initToken();

    addToLexeme(token, ch);
    finishToken(token, singleChars);

    pos->col++;

    return token;
}

Token* handleOperator(FILE* file, char ch, FilePos* pos) {
    Token* token = initToken();

    addToLexeme(token, ch);

    pos->col++;

    // just solving these options "==", "!=", "<=", ">=", "??"
    if (((ch == '=' || ch == '!' || ch == '<' || ch == '>') && (ch = fgetc(file)) == '=') ||
         (token->lexeme[0] == '?' && ch == '?')) {
        addToLexeme(token, ch);
        pos->col++;
    }

    finishToken(token, singleChars);
    return token;
}

Token* multilineString(FILE* file, FilePos* pos) {
    Token* token = initToken();
    int ch;
    int quoteCheck[3];

    while ((ch = fgetc(file)) != EOF) {
        if (ch != '\\') {
            if ((quoteCheck[0] = fgetc(file)) == '"') {
                if ((quoteCheck[1] = fgetc(file)) == '"')  {
                    if ((quoteCheck[2] = fgetc(file)) == '"')  {
                        pos->col += 4;

                        addToLexeme(token, ch);
                        finishToken(token, string);
                        return token;

                    } else { ungetc(quoteCheck[2], file); }
                } else { ungetc(quoteCheck[1], file); }
            } else { ungetc(quoteCheck[0], file); }
        }

        addToLexeme(token, ch);
        pos->col++;
    }

    // string ended with EOF insted of """
    finishToken(token, unknown);
    return token;
}

Token* singlelineString(FILE* file, FilePos* pos) {
    Token* token = initToken();
    int quoteCheck;
    int ch;

    while ((ch = fgetc(file)) != '\n' && ch != EOF) {
        if (ch != '\\') {
            if ((quoteCheck = fgetc(file)) == '"') {
                pos->col++;

                addToLexeme(token, ch);
                finishToken(token, string);
                return token;
            } else { ungetc(quoteCheck, file); }
        }
        addToLexeme(token, ch);
        pos->col++;
    }

    finishToken(token, unknown);
    return token;
}

Token* handleString(FILE* file, FilePos* pos) {
    bool multiline = false;
    int ch;

    pos->col++;

    // check for multiline strings
    if ((ch = fgetc(file)) == '"') {
         if ((ch = fgetc(file)) == '"')  {
             multiline = true;

         } else { ungetc(ch, file); }
    } else { ungetc(ch, file); }

    if ( multiline ) {
        return multilineString(file, pos);
    } else {
        return singlelineString(file, pos);
    }
}

Token* getToken(FILE* file) {
    if (!file) {
        Token* token = initToken();
        addToLexeme(token, EOF);
        finishToken(token, unknown);
        return token;
    }

    char ch;
    FilePos pos = {.col = 1, .line = 0};

    while ((ch = fgetc(file)) != EOF) {

        // whitespaces
        if (isspace(ch)) {
            if (ch == '\n') {
                pos.col = 0;
                pos.line++;
            }
            pos.col++;
        }

        // comments
        else if (ch == '/') {
            handleComments(file, ch);
        }

        // keywords and identifiers
        else if (isalpha(ch) || ch == '_') {
            return handleIdentifier(file, ch, &pos);
        }
        
        // numbers
        else if (isdigit(ch)) {
            return handleNumber(file, ch, &pos);
        }
        
        // operators
        else if (ch == '=' ||
                 ch == '+' ||
                 ch == '-' ||
                 ch == '*' ||
                 ch == '/' ||
                 ch == '<' ||
                 ch == '>' ||
                 ch == '!' ||
                 ch == '?' ) {
            return handleOperator(file, ch, &pos);
        }

        // single characters
        else if (ch == ':' ||
                 ch == ',' ||
                 ch == ';' ||
                 ch == '(' ||
                 ch == ')' ||
                 ch == '[' ||
                 ch == ']' ||
                 ch == '{' ||
                 ch == '}') {
            return handleSingleChars(file, ch, &pos);
        }

        else if (ch == '"') {
            return handleString(file, &pos);
        }

        // unknown characters
        else {
            Token* token = initToken();
            addToLexeme(token, ch);
            finishToken(token, unknown);
            return token;
        }
    }

    Token* token = initToken();
    addToLexeme(token, EOF);
    finishToken(token, unknown);
    return token;
}
