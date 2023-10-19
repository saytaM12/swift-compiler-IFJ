#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LEXEME_LEN 20

/*
 * 1 identifier = x, Pos, Bool, ReadWord, ...
 * 2 number = 10, -15, 67.62, ...
 * 3 operation = +, -, *, /, ...
 * 4 otherShit = =, {, }, <, >, (, ), ...
*/
typedef enum{
    identifier = 1,
    number = 2,
    operation = 3,
    otherShit = 4,
    unknown = 5
} Type;

/*
 * Type type = enumeration describing type of token,
 * char* lexeme = the token as string
 */
typedef struct {
    Type type;
    char* lexeme;
} Token;

/*
 * Get next token from file,
 * ,
 * FILE* file = file to read from,
 * returns Token structure of read token
 */
Token getToken(FILE* file);

/*
 * read from file while characters match an identifier pattern,
 * ('a' - 'z', '0' - '9', '_'),
 * ,
 * FILE* file = file from which to read,
 * char ch = first character already read (for simplicity),
 * int* col = column on which the character ch is,
 * returns Token structure of read token
 */
Token handleIdentifier(FILE* file, char ch, int* col);

/*
 * read from file while characters match a number pattern,
 * ('0' - '9'),
 *
 * FILE* file = file from which to read,
 * char ch = first character already read (for simplicity),
 * int* col = column on which the character ch is,
 * returns Token structure of read token
 */
Token handleNumber(FILE* file, char ch, int* col);


/*
 * Ignore comments,
 * ,
 * FILE* file = file from which to read,
 * char ch = first character already read (for simplicity),
 */
void handleComments(FILE* file, char ch);

Token handleShit(FILE* file, char ch, int* col);

Token handleOperator(FILE* file, char ch, int* col);
