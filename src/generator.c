#include "generator.h"

void generator_init(code_t *code) {
    code->last = NULL;
    code->first = NULL;
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
