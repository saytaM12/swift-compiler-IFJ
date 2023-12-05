#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_generator.h"

/**
 * Built-in functions
 *
 * all these functions return their resutls in %retval
 * and accept parameters as %0, %1, ..., %n for n paramaters
 */
// length(string) int
#define FUN_LENGTH               \
    "\n PUSHFRAME"               \
    "\n DEFVAR LF@%retval"       \
    "\n STRLEN LF@%retval LF@%0" \
    "\n POPFRAME"                \
    "\n RETURN"

// chr(int) char
#define chr                        \
    "\n LABEL chr"                 \
    "\n PUSHFRAME"                 \
    "\n DEFVAR LF@%retval"         \
    "\n INT2CHAR LF@%retval LF@%0" \
    "\n POPFRAME"                  \
    "\n RETURN"

// ord(str) int
#define ord                                         \
    "\n LABEL ord"                                  \
    "\n PUSHFRAME"                                  \
    "\n DEFVAR LF@%retval"                          \
    "\n MOVE LF@%retval int@0"                      \
    "\n DEFVAR LF@tmp_ord"                          \
    "\n STRLEN LF@tmp_ord LF@%0"                    \
    "\n JUMPIFEQ jump_ord_false$0 LF@tmp_ord int@0" \
    "\n STRI2INT LF@%retval LF@%0 int@0"            \
    "\n LABEL jump_ord_false$0"                     \
    "\n POPFRAME"                                   \
    "\n RETURN"

// substring(str, int, int) str
#define substring                                  \
    "\n LABEL substring"                           \
    "\n PUSHFRAME"                                 \
    "\n DEFVAR LF@%retval"                         \
    "\n DEFVAR LF@strlen"                          \
    "\n STRLEN LF@strlen LF@%0"                    \
    "\n DEFVAR LF@tmp1"                            \
    "\n LT LF@tmp1 LF@%1 int@0"                    \
    "\n JUMPIFEQ $substring$err LF@tmp1 bool@true" \
    "\n DEFVAR LF@tmp2"                            \
    "\n LT LF@tmp2 LF@%2 int@0"                    \
    "\n JUMPIFEQ $substring$err LF@tmp2 bool@true" \
    "\n DEFVAR LF@tmp3"                            \
    "\n DEFVAR LF@tmp3_2"                          \
    "\n SUB LF@tmp3_2 LF@strlen int@1"             \
    "\n GT LF@tmp3 LF@%1 LF@tmp3_2"                \
    "\n JUMPIFEQ $substring$err LF@tmp3 bool@true" \
    "\n DEFVAR LF@tmp4"                            \
    "\n GT LF@tmp4 LF@%2 LF@strlen"                \
    "\n JUMPIFEQ $substring$err LF@tmp4 bool@true" \
    "\n DEFVAR LF@tmp5"                            \
    "\n GT LF@tmp5 LF@%1 LF@%2"                    \
    "\n JUMPIFEQ $substring$err LF@tmp5 bool@true" \
    "\n JUMPIFEQ $substring$i_eq_j LF@%1 LF@%2"    \
    "\n MOVE LF@%retval string@"                   \
    "\n DEFVAR LF@i"                               \
    "\n MOVE LF@i LF@%1"                           \
    "\n DEFVAR LF@temp_ch"                         \
    "\n LABEL $substring$loop"                     \
    "\n JUMPIFEQ $substring$end LF@i LF@%2"        \
    "\n GETCHAR LF@temp_ch LF@%0 LF@i"             \
    "\n CONCAT LF@%retval LF@%retval LF@temp_ch"   \
    "\n ADD LF@i LF@i int@1"                       \
    "\n JUMP $substring$loop"                      \
    "\n LABEL $substring$end"                      \
    "\n POPFRAME"                                  \
    "\n RETURN"                                    \
    "\n LABEL $substring$i_eq_j"                   \
    "\n MOVE LF@%retval string@"                   \
    "\n POPFRAME"                                  \
    "\n RETURN"                                    \
    "\n LABEL $substring$err"                      \
    "\n MOVE LF@%retval nil@nil"                   \
    "\n POPFRAME"                                  \
    "\n RETURN"

// readString() string
// returns string stored in %retval
#define FUN_READ_STRING         \
    "\n LABEL readString"       \
    "\n PUSHFRAME"              \
    "\n DEFVAR LF@%retval"      \
    "\n READ LF@%retval string" \
    "\n POPFRAME"               \
    "\n RETURN"

// readInt() int
// returns int stored in %retval
#define FUN_READ_INT         \
    "\n LABEL readInt"       \
    "\n PUSHFRAME"           \
    "\n DEFVAR LF@%retval"   \
    "\n READ LF@%retval int" \
    "\n POPFRAME"            \
    "\n RETURN"

// readDouble() float
// returns float stored in %retval
#define FUN_READ_DOUBLE        \
    "\n LABEL readInt"         \
    "\n PUSHFRAME"             \
    "\n DEFVAR LF@%retval"     \
    "\n READ LF@%retval float" \
    "\n POPFRAME"              \
    "\n RETURN"

// TODO: jak dostaneme params do tohoto kodu?
void fun_write(int num_of_params)
{
    for (int i = 0; i < num_of_params; i++)
    {
    }
}
