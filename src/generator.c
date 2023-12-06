#include "generator.h"
#include "parser.h"

const char *typeLookup[] = {
    "int",
    "float",
    "string",
};

code_t generator_code_init() {
    code_t code;
    code.last = NULL;
    code.first = NULL;

    generator_addLineEnd(&code, ".IFJcode23");
    generator_addLineEnd(&code, "JUMP $$main");

    return code;
}

instruction_t *generator_ins_init() {
    instruction_t *ins = malloc(sizeof(instruction_t));
    ins->currScope = calloc(sizeof(scope_t), 1);

    return ins;
}

void generator_code_destroy(code_t *code) {
    if (!code->first) {
        return;
    }

    line_t *node = code->first;
    line_t *oldNode;

    while (node) {
        oldNode = node;
        node = oldNode->next;
        free(oldNode->line);
        free(oldNode);
    }

    code->first = NULL;
    code->last = NULL;
}

void generator_ins_destroy(instruction_t *ins) {
    scope_t *scope = ins->currScope;
    scope_t *nextScope;
    while (scope) {
        nextScope = scope->next;
        free(scope->name);
        free(scope);
        scope = nextScope;
    }
    switch (ins->instructionType) {
        case funDef:
            for (int i = 0; i < ins->funDef.paramNum; i++) {
                free(ins->funDef.parameters[i]->name);
                free(ins->funDef.parameters[i]->id);
                free(ins->funDef.parameters[i]);
            }
            free(ins->funDef.parameters);
            free(ins->funDef.name);
            break;
        case funCal:
            for (int i = 0; i < ins->funCal.paramNum; i++) {
                free(ins->funCal.parameters[i]);
            }

            free(ins->funCal.parameters);
            free(ins->funCal.name);
        break;
        case varDef:
        break;
        case assign:
        break;
        case whileLoop:
        break;
        case ifExpr:
        break;
    }
    free(ins);
}

void generator_ins_push_scope(instruction_t *ins, int offset, char *name) {
    ins->totalOffset += offset;
    scope_t *newScope = malloc(sizeof(scope_t));
    newScope->offset = offset;
    newScope->name = malloc(strlen(name) + 1);
    strcpy(newScope->name, name);
    newScope->next = ins->currScope;
    ins->currScope = newScope;
}

void generator_ins_add_to_offset(instruction_t *ins, int offset) {
    ins->totalOffset += offset;
    ins->currScope->offset += offset;
}

char *generator_ins_get_current_scope_name(instruction_t ins) {
    if (ins.currScope) {
        return ins.currScope->name;
    }
    return "";
}

void generator_ins_pop_scope(instruction_t *ins) {
    ins->totalOffset -= ins->currScope->offset;
    scope_t * oldScope = ins->currScope;
    ins->currScope = oldScope->next;
    free(oldScope);
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

    line_t *new = calloc(sizeof(line_t), 1);
    if (!new) {
        fputs("malloc fail", stderr);
        return -2;
    }

    new->line = malloc(strlen(line) + 1);
    strcpy(new->line, line);

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

void translateFunDef() {
    char *line = malloc(1 + strlen("LABEL $") + strlen(ins->funDef.name));
    sprintf(line, "LABEL $%s", ins->funDef.name);

    generator_addLineEnd(&code, line);
    generator_addLineEnd(&code, "PUSHFRAME");

    char *var = malloc(1);
    line = realloc(line, 1 + strlen("DEFVAL LF@$") + strlen(ins->funDef.name) + strlen("$retval"));
    sprintf(line, "DEFVAR LF@$%s$retval", ins->funDef.name);
    generator_addLineEnd(&code, line);

    for (int i = 0; i < ins->funDef.paramNum; i++) {
        line = realloc(line, 2 + strlen("DEFVAR LF@$") + strlen(ins->funDef.name) + strlen("$param1") + (int)log10((double)i?i:1));
        var = realloc(var, 3 + strlen("LF@$%s$%d") + strlen(ins->funDef.name) + (int)log10((double)i?i:1));
        sprintf(var, "LF@$%s$%d", ins->funDef.name, i);
        sprintf(line, "DEFVAR %s", var);
        generator_addLineEnd(&code, line);

        line = realloc(line, 3 + strlen("MOVE LF@$") + strlen(ins->funDef.name) + strlen("$param1") + 2*(int)log10((double)i?i:1) + strlen("LF@%1"));
        sprintf(line, "MOVE %s LF@%%%d", var, i);
        generator_addLineEnd(&code, line);
    }

    line = realloc(line, 1 + strlen("LABEL $") + strlen(ins->funDef.name) + strlen("$end"));
    sprintf(line, "LABEL $%s$end", ins->funDef.name);
    generator_addLineEnd(&code, line);
    generator_addLineEnd(&code, "RETURN");
    generator_addLineEnd(&code, "POPFRAME");
    generator_ins_push_scope(ins, 2, ins->funDef.name);

    free(line);
    free(var);

    generator_ins_destroy(ins);
    ins = generator_ins_init();
}

void translateFunCal() {
    generator_addLineFromEnd(&code, "PUSHFRAME", ins->totalOffset);

    char *line = malloc(1);

    for (int i = 0; i < ins->funCal.paramNum; i++) {
        line = realloc(line, 2 + strlen("DEFVAL TF@%") + (int)log10((double)i?i:1));
        sprintf(line, "DEFVAL TF@%%%d", i);
        generator_addLineFromEnd(&code, line, ins->totalOffset);

        line = realloc(line, 2 + strlen("MOVE TF@% ") + (int)log10((double)i?i:1) + strlen(ins->funCal.parameters[i]));
        sprintf(line, "MOVE TF@%%%d %s", i, ins->funCal.parameters[i]);
        generator_addLineFromEnd(&code, line, ins->totalOffset);
    }

    line = realloc(line, 1 + strlen("CALL $") + strlen(ins->funCal.name));
    sprintf(line, "CALL $%s", ins->funCal.name);
    generator_addLineFromEnd(&code, line, ins->totalOffset);

    free(line);

    generator_ins_destroy(ins);
    ins = generator_ins_init();
}

void translateVarDEF() {
    int max_len;
    char *line;

    //if (ins->varDef.local) {

    if (ins->currScope) {
        generator_addLineFromEnd(&code,
                strcat(strcat("DEFVAR LF@$", ins->currScope->name), strcat("$", ins->varDef.name)),
                ins->totalOffset);
    } else {
        generator_addLineFromEnd(&code, strcat("DEFVAR GF@$", ins->varDef.name), ins->totalOffset);
    }

        /*
    }
    else {
        generator_addLineFromEnd(&code, strcat("DEFVAR GF@", ins->varDef.name), ins->totalOffset);
    }
    */

    if (ins->varDef.value) {
        max_len = strlen(ins->varDef.value) + strlen(ins->varDef.name) + 20;
        line = malloc(sizeof(char) * max_len);
        snprintf(line, max_len, "MOVE LF@%s %s@%s", ins->varDef.name, typeLookup[ins->varDef.type], ins->varDef.value);
        generator_addLineFromEnd(&code, line, ins->totalOffset);
    }
}

void translateAssign() {
    int max_len;
    char *line;
    max_len = strlen(ins->assign.from) + strlen(ins->assign.to) + 20;
    line = malloc(sizeof(char) * max_len);
    snprintf(line, max_len, "MOVE LF@%s LF@%s", ins->assign.to, ins->assign.from);
    generator_addLineFromEnd(&code, line, ins->totalOffset);
}

void generator_translate() {
    switch (ins->instructionType) {
        case funDef:
            translateFunDef();
            break;
        case funCal:
            translateFunCal();
            break;
        case varDef:
            translateVarDEF();
            break;
        case assign:
            translateAssign();
            break;
        case whileLoop:
            break;
        case ifExpr:
            break;
        default:
            break;
    }
}
