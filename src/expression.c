#include "expression.h"
#include "tokenizer.h"
#include "parser.h"

// +-   */   !=<>   !   ??   (   )   i   $
int precTable[9][9] = {
    {R, S, R, S, R, S, R, S, R},     // +-
    {R, R, R, S, R, S, R, S, R},     // */
    {S, S, Err, S, R, S, R, S, R},   // !=<>
    {R, R, R, R, R, S, R, R, R},     // !
    {S, S, S, S, R, S, S, S, R},     // ??
    {S, S, S, S, S, S, Eq, S, Err},  // (
    {R, R, R, R, R, Err, R, Err, R}, // )
    {R, R, R, R, R, Err, R, Err, R}, // i
    {S, S, S, S, S, S, Err, S, Fin}  // $
};

expression_list *expression_list_create()
{
    expression_list *list = (expression_list *)malloc(sizeof(expression_list));
    list->head = NULL;
    list->active = NULL;
    return list;
}

void disposeValue(expression_value *value)
{
    if (value->left != NULL)
    {
        disposeValue(value->left);
    }
    if (value->right != NULL)
    {
        disposeValue(value->right);
    }
    if (value->value != NULL)
    {
        free(value->value);
    }
    free(value);
}

void expression_list_dispose(expression_list *list)
{
    expression_element *node = list->head;
    while (node != NULL)
    {
        disposeValue(node->value);
        expression_element *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

void expression_list_insert(expression_list *list, expression_value *value)
{
    if (list->head == NULL)
    {
        expression_element *tmp = (expression_element *)malloc(sizeof(expression_element));
        if (tmp == NULL)
        {
            printf("Error: malloc failed\n");
        }
        tmp->prev = NULL;
        tmp->value = value;
        tmp->next = NULL;
        list->head = tmp;
        list->active = tmp;
        return;
    }
    else
    {
        expression_element *tmp = (expression_element *)malloc(sizeof(expression_element));
        if (tmp == NULL)
        {
            printf("Error: malloc failed\n");
        }
        while (list->active->next != NULL)
        {
            list->active = list->active->next;
        }
        tmp->prev = list->active;
        tmp->value = value;
        tmp->next = NULL;
        list->active->next = tmp;
        return;
    }
}

void printExprList(expression_list *stack)
{
    expression_element *node = stack->head;
    while (node != NULL)
    {
        printValue(node->value, 0);
        node = node->next;
        printf("\n");
    }
    printf("\n");
}

void printValue(expression_value *value, int level)
{
    if (value == NULL)
    {
        printf("\n");
        return;
    }
    printValue(value->right, level + 1);

    for (int i = 0; i < level; i++)
    {
        printf("    "); // Odsazení podle úrovně ve stromu
    }
    printf("%s\n", value->value);
    printValue(value->left, level + 1);
    return;
}

expression_value *expression_list_pop(expression_list *list)
{
    expression_element *node = list->head;
    expression_element *prev = NULL;
    if (node == NULL)
    {
        return NULL;
    }
    while (node->next != NULL)
    {
        prev = node;
        node = node->next;
    }
    expression_value *value = node->value;
    if (prev == NULL)
    {
        list->head = NULL;
        list->active = NULL;
    }
    else
    {
        prev->next = NULL;
        list->active = prev;
    }
    return value;
}

expression_value *expression_last(expression_list *list)
{
    expression_element *node = list->head;
    if (node == NULL)
    {
        return NULL;
    }
    expression_value *value = node->value;
    while (node->next != NULL)
    {
        if (node->next->value->index != Dollar)
        {
            value = node->next->value;
        }
        // value = node->next->value;
        node = node->next;
    }
    return value;
}

prec_index getIndex(char *type)
{
    if (strcmp(type, "+") == 0 || strcmp(type, "-") == 0)
    {
        return PlusMinus;
    }
    else if (strcmp(type, "*") == 0 || strcmp(type, "/") == 0)
    {
        return MultiplyDivide;
    }
    else if (strcmp(type, "!=") == 0 || strcmp(type, "==") == 0 || strcmp(type, "<") == 0 || strcmp(type, ">") == 0 || strcmp(type, "<=") == 0 || strcmp(type, ">=") == 0)
    {
        return Rel;
    }
    else if (strcmp(type, "!") == 0)
    {
        return ExMark;
    }
    else if (strcmp(type, "??") == 0)
    {
        return QuestionMark;
    }
    else if (strcmp(type, "(") == 0)
    {
        return OpenBracket;
    }
    else if (strcmp(type, ")") == 0)
    {
        return CloseBracket;
    }
    else if (strcmp(type, "i") == 0)
    {
        return Identifier;
    }
    else if (strcmp(type, "$") == 0)
    {
        return Dollar;
    }
    else
    {
        return -1;
    }
}

expression_value *expression_value_create(Token *token)
{
    expression_value *value = malloc(sizeof(expression_value));
    char *type = malloc(sizeof(char) * ((int)token->lexlen) + 1);
    memcpy(type, token->lexeme, token->lexlen);
    type[token->lexlen] = '\0';
    if (token->type == number)
    {
        value->type = num;
        value->index = Identifier;
    }
    else if (token->type == string)
    {
        value->type = str;
        value->index = Identifier;
    }
    else if (token->type == identifier)
    {
        value->type = str;
        value->index = Identifier;
    }
    else if (token->type == numberFloat)
    {
        value->type = doub;
        value->index = Identifier;
    }
    else
    {
        value->type = getIndex(type);
        value->index = getIndex(type);
    }
    value->value = type;
    value->left = NULL;
    value->right = NULL;
    return value;
}

int reduceOp(expression_list *stack, expression_element *node)
{
    node->value->index = Dollar;
    node->value->action = R;
    node->value->right = expression_list_pop(stack);
    node->value = expression_list_pop(stack);
    node->value->left = expression_list_pop(stack);
    expression_list_insert(stack, node->value);
    return 0;
}

int reduce(expression_list *stack)
{
    expression_element *lastShift = NULL;
    expression_element *node = stack->head;
    while (node != NULL)
    {
        if (node->value->action == S)
        {
            lastShift = node;
        }
        node = node->next;
    }
    if (lastShift == NULL)
    {
        return 2;
    }
    if (lastShift->next->value->index == Dollar)
    {
        node = lastShift;
    }
    else
    {
        node = lastShift->next;
    }
    if (node->value->index == Identifier)
    {
        node->value->index = Dollar;
        printf("E -> i\n");
        return 0;
    }
    else if (strcmp(node->value->value, "+") == 0)
    {
        reduceOp(stack, node);
        if (node->value->left == NULL || node->value->right == NULL)
        {
            return 2;
        }
        if (node->value->left->type == str && node->value->right->type == str)
        {
            node->value->type = str;
        }
        else if (node->value->left->type==num && node->value->right->type==num)
        {
            node->value->type = num;
        }
        else if ((node->value->left->type == doub || node->value->left->type == num) && (node->value->right->type == doub || node->value->right->type == num))
        {
            node->value->type = doub;
        }
        else
        {
            return 7;
        }
        printf("E -> E + E\n");
    }
    else if (node->value->index == MultiplyDivide || strcmp(node->value->value, "-"))
    {
        reduceOp(stack, node);
        if (node->value->left == NULL || node->value->right == NULL)
        {
            return 2;
        }
        if (node->value->left->type == num && node->value->right->type == num)
        {
            node->value->type = num;
        }
        else if ((node->value->left->type == doub || node->value->left->type == num) && (node->value->right->type == doub || node->value->right->type == num))
        {
            node->value->type = doub;
        }
        else
        {
            return 7;
        }
        printf("E -> E op E\n");
    }
    else if (strcmp(node->value->value, "==") == 0)
    {
        reduceOp(stack, node);
        if (node->value->left == NULL || node->value->right == NULL)
        {
            return 2;
        }
        else if ((node->value->left->type == num || node->value->left->type == doub)&& (node->value->right->type == num || node->value->right->type == doub))
        {
            node->value->type = boo;
        }
        else if (node->value->left->type != node->value->right->type){
            return 7;
        }
    }

    else if (node->value->index == Rel)
    {
        reduceOp(stack, node);
        if (node->value->left == NULL || node->value->right == NULL)
        {
            return 2;
        }
        if ((node->value->left->type != node->value->right->type) && (node->value->left->type != nil) && (node->value->right->type != nil))
        {
            return 7;
        }
        node->value->type = boo;
    }
    else if (node->value->index == QuestionMark)
    {
        reduceOp(stack, node);
        if (node->value->left == NULL || node->value->right == NULL)
        {
            return 2;
        }
        if (node->value->left->type == nil)
        {
            node->value->type = node->value->right->type;
        }
        else
        {
            node->value->type = node->value->left->type;
        }
    }
    else
    {
        return 2;
    }
    return 0;
}

int bottomUp(Token *token, FILE *fp, expression_value **returningValue)
{
    expression_value *retVal = NULL;
    expression_list *stack = expression_list_create();
    char *dollar = malloc(sizeof(char) * 2);
    strcpy(dollar, "$");
    expression_value *dollarVal = malloc(sizeof(expression_value));
    dollarVal->action = Fin;
    dollarVal->value = dollar;
    dollarVal->left = NULL;
    dollarVal->right = NULL;
    dollarVal->index = Dollar;
    int openBrackets = 0;
    int x;
    expression_list_insert(stack, dollarVal);
    // token = new_token(fp, token);
    if (!(token->type != identifier || token->type != number || token->type != string || token->type != numberFloat || token->type != singleChars))
    {
        return 2;
    }

    while (token->type == identifier || token->type == number || token->type == string || token->type == operation || token->type == singleChars || token->type == numberFloat)
    {
        if (token->type == unknown)
        {
            break;
        }
        expression_value *value = expression_value_create(token);
        expression_value *last = expression_last(stack);
        int action = precTable[last->index][value->index];
        printf("%s\n", value->value);
        if (action == S)
        {
            last->action = S;
            expression_list_insert(stack, value);
            token = new_token(fp, token);
            if (token->lexeme[0] == '(')
            {
                openBrackets++;
            }
            else if (token->lexeme[0] == ')')
            {
                openBrackets--;
            }
        }
        else if (action == R)
        {
            if ((x = reduce(stack)) != 0)
            {
                return x;
            }
        }
        else if (action == Err)
        {
            if (openBrackets == 0)
            {
                free(value);
                break;
            }
            free(value);
            return 2;
        }
        else if (action == Eq)
        {
            expression_element *tmp = stack->head;
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            tmp->value = expression_list_pop(stack);
            disposeValue(expression_list_pop(stack));
            expression_list_insert(stack, tmp->value);
            free(value);
            token = new_token(fp, token);
        }
        else if (action == Fin)
        {
            break;
        }
        else
        {
            printf("Error: syntax error\n");
            free(value);
            return 2;
        }

        if (token->lexeme[0] == EOF)
        {
            break;
        }
    }
    if (stack->head->next != NULL)
    {
        while (stack->head->next->next != NULL)
        {
            if ((x = reduce(stack)) == 1)
            {
                printf("Error: Syntax error\n");
                return 2;
            }
            else if (x == 2)
            {
                printf("Error: Semantics error\n");
                return 7;
            }
        }
    }
    retVal = expression_list_pop(stack);
    expression_list_dispose(stack);
    *returningValue=retVal;
    returnToken(token,fp);
    return 0;
}
