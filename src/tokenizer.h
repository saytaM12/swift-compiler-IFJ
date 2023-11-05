#include <stdlib.h>

#define DEFAULT_LEXEME_LEN 20

/*
 * 1 identifier = x, Pos, Bool, ReadWord, ...
 * 2 keyword = Double, else, func, if, Int, let, nil, return, String, var, while
 * 3 number = 10, -15, 67.62, ...
 * 4 operation = +, -, *, /, <=, ==, !, ...
 * 5 singleChars = {, }, [, ], (, ), ',', :, ;,
 * 6 string = "Hello, World", ...
 * 7 unknown
*/
typedef enum{
    identifier = 1,
    keyword = 2,
    number = 3,
    operation = 4,
    singleChars = 5,
    string = 6,
    unknown = 7
} Type;

/*
 * Type type = enumeration describing type of token,
 * char* lexeme = the token as string
 */
typedef struct {
    char* lexeme;
    Type type;
    size_t size;
    size_t lexlen;
} Token;

/*
 * creates a new Token and returns a pointer to it,
 */
Token* initToken();

/*
 * adds a character to lexeme of token,
 * ,
 * Token* token = token to which to add a character,
 * char lexeme = character which to add
 */
void addToLexeme(Token* token, char ch);

/*
 * finalizes token for return conditions and adds a type to it,
 * ,
 * Token* token = token which to work with,
 * Type type = type what tke token should be
 */
void finishToken(Token* token, Type type);

/*
 * destroys token so it cannot be used later and frees memory,
 * ,
 * Token* token = token which to free
 */
void destroyToken(Token* token);

/*
 * changes type of token passed,
 * ,
 * Token* token = token that's type should change,
 * Type type = type to change token's type to,
 */
void changeTokenType(Token* token, Type type);
