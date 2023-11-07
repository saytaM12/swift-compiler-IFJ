#ifndef LEXICALDEFINED
#define LEXICALDEFINED
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "tokenizer.h"

#define NUMOFKEYWORDS 8
#define NUMOFVARIABLETYPES 3

/*
 * indicates at which position file is being read,
 * ,
 * int col = column,
 * int line = line
 */
typedef struct {
    int col;
    int line;
} FilePos;

/*
 * Get next token from file,
 * ,
 * FILE* file = file to read from,
 * returns Token structure of read token
 */
Token* getToken(FILE* file);

/*
 * read from file while characters match an identifier pattern,
 * ('a' - 'z', '0' - '9', '_'),
 * ,
 * FILE* file = file from which to read,
 * char ch = first character already read (for simplicity),
 * FilePos* pos = position on which the character ch is,
 * returns Token structure of read token
 */
Token* handleIdentifier(FILE* file, char ch, FilePos* pos);

/*
 * read from file while characters match a number pattern,
 * ('0' - '9'),
 * ,
 * FILE* file = file from which to read,
 * char ch = first character already read (for simplicity),
 * FilePos* pos = position on which the character ch is,
 * returns Token structure of read token
 */
Token* handleNumber(FILE* file, char ch, FilePos* pos);

/*
 * read characters between two quatition marks from file,
 * ("..."), ("Hello, World"),
 * ,
 * FILE* file = file from which to read,
 * FilePos* pos = position on which the character first qutation mark is,
 * returns Token structure of read token
 */
Token* handleString(FILE* file, FilePos* pos);

/*
 * Ignore comments,
 * ,
 * FILE* file = file from which to read,
 * char ch = first character already read (for simplicity),
 */
void handleComments(FILE* file, char ch);

/*
 * return a token containing a single character passed into this function,
 * (( { } [ ] ( ) , : ; )),
 * ,
 * FILE* file = file from which to read,
 * char ch = character which to return
 * FilePos* pos = position on which the character first qutation mark is,
 * returns Token structure of read token
 */
Token* handleSingleChars(FILE* file, char ch, FilePos* pos);

/*
 * reads an operator from the file
 * (( + - * / <= >= == ?? ! != )),
 * ,
 * FILE* file = file from which to read,
 * char ch = first character of the operator already read
 * FilePos* pos = position on which the character first qutation mark is,
 * returns Token structure of read token
 */
Token* handleOperator(FILE* file, char ch, FilePos* pos);
<<<<<<< HEAD
#endif //LEXICALDEFINED
=======
#endif //LEXICALDEFINED
>>>>>>> 82234ed5ea03b87f549b1ca39b7835a40daa0788
