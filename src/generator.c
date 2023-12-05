#include "generator.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

code_t generator_code_init() {
    code_t code;
    code.last = NULL;
    code.first = NULL;

    generator_addLineEnd(&code, ".IFJcode23");
    generator_addLineEnd(&code, "JUMP $$main");

    return code;
}

instruction_t *generator_ins_init() {
    return malloc(sizeof(instruction_t));
}

void generator_destroy(code_t *code) {
    if (!code->first) {
        return;
    }

    line_t *next = code->first;

    while (next) {
        line_t *node = next;
        next = node->next;
        free(node);
    }

    code->first = NULL;
    code->last = NULL;
}

int generator_addLineEnd(code_t *code, char *line) {
    return generator_addLineFromEnd(code, line, 0);
}

int generator_addLineFromEnd(code_t *code, char *line, int offset) {
    line_t *previous = code->last;

    // the list is not empty
    if (previous) {
        for (int i = 0; i < offset; i++) {
            if (!(previous = previous->prev)) {
                fputs("trying to add from end more than length of list", stderr);
                return -1;
            }
        }
    }

    line_t *new = malloc(sizeof(line_t));
    if (!new) {
        fputs("malloc fail", stderr);
        return -2;
    }

    new->line = line;

    if (previous) {
        new->prev = previous;
        new->next = previous->next;
        previous->next = new;

        if (previous == code->last) {
            code->last = new;
        }
    } else { // list was empty ('previous' was NULL pointer)
        new->next = new->prev = NULL;
        code->first = code->last = new;
    }

    return 0;
}

void generator_write(FILE *file, code_t code) {
    line_t *line = code.first;

    while (line) {
        fprintf(file, "%s\n", line->line);
        line = line->next;
    }
}

void generator_translate(code_t code) {
    int max_len;
    char *line;
    switch (ins->instructionType) {
    case funDef:
        break;
    case funCal:
        break;
    case varDef:
        if (ins->varDef.local) {
            generator_addLineEnd(&code, strcat("DEFVAR LF@", ins->varDef.name));
        }
        else {
            generator_addLineEnd(&code, strcat("DEFVAR GF@", ins->varDef.name));
        }

        if (ins->varDef.value) {
            max_len = strlen(ins->varDef.value) + strlen(ins->varDef.name) + 20;
            line = malloc(sizeof(char) * max_len);
            snprintf(line, max_len, "MOVE LF@%s %s@%s", ins->varDef.name, varTypeToString(ins->varDef.type), ins->varDef.value);
            generator_addLineEnd(&code, line);
        }
        break;
    case assign:
        max_len = strlen(ins->assign.from) + strlen(ins->assign.to) + 20;
        line = malloc(sizeof(char) * max_len);
        snprintf(line, max_len, "MOVE LF@%s LF@%s", ins->assign.to, ins->assign.from);
        generator_addLineEnd(&code, line);
        break;
    case whileLoop:
        break;
    case ifExpr:
        break;
    default:
        break;
    }
}

char *varTypeToString(varialbeType_e type) {
    switch (type) {
    case 1:
        return "int";
    case 2:
        return "float";
    case 3:
        return "string";
    default:
        return "unknown";
    }
}
