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
            free(ins->varDef.name);
            free(ins->varDef.value);
        break;
        case assign:
            free(ins->assign.from);
            free(ins->assign.to);
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

    if (ins->varDef.value) {
        max_len = strlen(ins->varDef.value) + strlen(ins->varDef.name) + 20;
        line = malloc(sizeof(char) * max_len);
        snprintf(line, max_len, "MOVE LF@%s %s@%s", ins->varDef.name, typeLookup[ins->varDef.type], ins->varDef.value);
        generator_addLineFromEnd(&code, line, ins->totalOffset);
    }

    generator_ins_destroy(ins);
    ins = generator_ins_init();
}

void translateAssign() {
    int max_len;
    char *line;
    max_len = strlen(ins->assign.from) + strlen(ins->assign.to) + 20;
    line = malloc(sizeof(char) * max_len);
    snprintf(line, max_len, "MOVE LF@%s LF@%s", ins->assign.to, ins->assign.from);
    generator_addLineFromEnd(&code, line, ins->totalOffset);

    generator_ins_destroy(ins);
    ins = generator_ins_init();
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

void translateExpression(expression_value *expr_val, int lineFromEnd) {
    if (expr_val->type == num || expr_val->type == doub) {
        postOrderTraversal(expr_val, expr_val->type, lineFromEnd);
    }
    else if (expr_val->type == str)
    {

    }
}

void postOrderTraversal(expression_value *curr, int type, int fromEnd) {
    if (curr->left) {
        postOrderTraversal(curr->left, type, fromEnd);
    }
    if (curr->right) {
        postOrderTraversal(curr->right, type, fromEnd);
    }
    
    if (isNumber(curr->value)) {
        char *line = malloc(1 + strlen("PUSHS int@") + strlen(curr->value));
        sprintf(line, "PUSHS int@%s", curr->value);
        generator_addLineFromEnd(&code, line, fromEnd);

        return;
    } 
    // checks that both types are the same, otherwise converts int to float
    codeTypeCheck(fromEnd);

    if (strcmp(curr->value, "+") == 0) {
        generator_addLineFromEnd(&code, "ADDS", fromEnd);
    } else if (strcmp(curr->value, "-") == 0) {
        generator_addLineFromEnd(&code, "SUBS", fromEnd);
    } else if (strcmp(curr->value, "*") == 0) {
        generator_addLineFromEnd(&code, "MULS", fromEnd);
    } else if (strcmp(curr->value, "/") == 0) {
        if (type == doub) {
            generator_addLineFromEnd(&code, "DIVS", fromEnd);
        }
        else if (type == num) {
            generator_addLineFromEnd(&code, "IDIVS", fromEnd);
        }
    } else if (strcmp(curr->value, ">") == 0) {
        generator_addLineFromEnd(&code, "GTS", fromEnd);
    } else if (strcmp(curr->value, "<") == 0) {
        generator_addLineFromEnd(&code, "LTS", fromEnd);
    } else if (strcmp(curr->value, ">=") == 0) {
        generator_addLineFromEnd(&code, "PUSHFRAME", fromEnd);
        generator_addLineFromEnd(&code, "CREATEFRAME", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%tmp1", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "POPS TF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "POPS TF@%tmp1", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%res_gt", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%res_eq", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%res", fromEnd);
        generator_addLineFromEnd(&code, "GT TF@%res_gt LF@%tmp1 LF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "EQ TF@%res_eq LF@%tmp1 LF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "OR TF@%res TF@%res_gt TF@%res_eq", fromEnd);
        generator_addLineFromEnd(&code, "PUSHS TF@%res", fromEnd);
        generator_addLineFromEnd(&code, "POPFRAME", fromEnd);
    } else if (strcmp(curr->value, "<=") == 0) {
        generator_addLineFromEnd(&code, "PUSHFRAME", fromEnd);
        generator_addLineFromEnd(&code, "CREATEFRAME", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%tmp1", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "POPS TF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "POPS TF@%tmp1", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%res_lt", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%res_eq", fromEnd);
        generator_addLineFromEnd(&code, "DEFVAR TF@%res", fromEnd);
        generator_addLineFromEnd(&code, "LT TF@%res_lt LF@%tmp1 LF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "EQ TF@%res_eq LF@%tmp1 LF@%tmp2", fromEnd);
        generator_addLineFromEnd(&code, "OR TF@%res TF@%res_lt TF@%res_eq", fromEnd);
        generator_addLineFromEnd(&code, "PUSHS TF@%res", fromEnd);
        generator_addLineFromEnd(&code, "POPFRAME", fromEnd);
    } else if (strcmp(curr->value, "!=") == 0) {
        generator_addLineFromEnd(&code, "EQS", fromEnd);
        generator_addLineFromEnd(&code, "NOTS", fromEnd);
    } else if (strcmp(curr->value, "==") == 0) {
        generator_addLineFromEnd(&code, "EQS", fromEnd);
    } else if (strcmp(curr->value, "!") == 0) {
        generator_addLineFromEnd(&code, "NOTS", fromEnd);
    }
}

int isNumber(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10);

    return (*str != '\0' && *endptr == '\0');
}

void codeTypeCheck(int fromEnd) {
    generator_addLineFromEnd(&code, "PUSHFRAME", fromEnd);
    generator_addLineFromEnd(&code, "CREATEFRAME", fromEnd);
    generator_addLineFromEnd(&code, "DEFVAR TF@val1", fromEnd);
    generator_addLineFromEnd(&code, "DEFVAR TF@val2", fromEnd);
    generator_addLineFromEnd(&code, "POPS TF@val1", fromEnd);
    generator_addLineFromEnd(&code, "POPS TF@val2", fromEnd);
    generator_addLineFromEnd(&code, "DEFVAR TF@type1", fromEnd);
    generator_addLineFromEnd(&code, "DEFVAR TF@type2", fromEnd);
    generator_addLineFromEnd(&code, "TYPE TF@type1 TF@val1", fromEnd);
    generator_addLineFromEnd(&code, "TYPE TF@type2 TF@val2", fromEnd);
    generator_addLineFromEnd(&code, "JUMPIFEQ $types_eq TF@type1 TF@type2", fromEnd);
    generator_addLineFromEnd(&code, "JUMPIFNEQ $val1_is_int TF@type1 string@int", fromEnd);
    generator_addLineFromEnd(&code, "INT2FLOAT TF@val1 TF@val1", fromEnd);
    generator_addLineFromEnd(&code, "JUMP $types_eq", fromEnd);
    generator_addLineFromEnd(&code, "LABEL $val1_is_int", fromEnd);
    generator_addLineFromEnd(&code, "INT2FLOAT TF@val2 TF@val2", fromEnd);
    generator_addLineFromEnd(&code, "LABEL $types_eq", fromEnd);
    generator_addLineFromEnd(&code, "PUSHS TF@val2", fromEnd);
    generator_addLineFromEnd(&code, "PUSHS TF@val1", fromEnd);
    generator_addLineFromEnd(&code, "POPFRAME", fromEnd);
}
