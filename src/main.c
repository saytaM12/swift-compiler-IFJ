#include "lexical.h"
#include "parser.h"
#include "generator.h"
#include "expression.h"
#include "symstack.h"
#include "tokenizer.h"

int main(void) {
    parse_prog();
    FILE *file = stdout;
    generator_write(file, code);
    fclose(file);
    generator_code_destroy(&code);
}
