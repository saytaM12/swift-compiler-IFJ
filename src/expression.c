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
        printValue(node->value,0);
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
    printValue(value->right , level + 1);

    for (int i = 0; i < level; i++) {
        printf("    ");  // Odsazení podle úrovně ve stromu
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
        node=node->next;

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
    if (token->type==number||token->type==string||token->type==identifier)
        {
            value->type = Identifier;
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

void reduce(expression_list *stack){
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
        printf("Error: syntax error\n");
        return;
    }
    if (lastShift->next->value->index == Dollar)
    {
        node=lastShift;
    }
    else
    {
        node = lastShift->next;
    }
    // node = lastShift->next;
    
    if (node->value->index==Identifier)
    {
        node->value->index = Dollar;
        lastShift = R;
        printf("E -> i\n");
        return;
    }
    else if (node->value->index==PlusMinus||node->value->index==MultiplyDivide||node->value->index==Rel||node->value->index==QuestionMark)
    {
        node->value->index = Dollar;
        node->value->action = R;
        node->value->right = expression_list_pop(stack);
        node->value = expression_list_pop(stack);
        node->value->left = expression_list_pop(stack);
        expression_list_insert(stack, node->value);
        printf("E -> E + E\n");
    }
    else
    {
        printf("Nejaky dalsi index: %d\n",node->value->index);
    }
    return;
}

expression_value *bottomUp(Token *token,FILE *fp)
{
    expression_value *retVal=NULL;
    // FILE *fp = fopen("input.swift", "r");
    expression_list *stack = expression_list_create();
    char *dollar = malloc(sizeof(char) * 2);
    strcpy(dollar, "$");
    expression_value *dollarVal = malloc(sizeof(expression_value));
    dollarVal->type = Dollar;
    dollarVal->action = Fin;
    dollarVal->value = dollar;
    dollarVal->left = NULL;
    dollarVal->right = NULL;
    dollarVal->index = Dollar;
    int openBrackets = 0;
    expression_list_insert(stack, dollarVal);
    // token = new_token(fp, token);
    if (!(token->type!=identifier||token->type != number||token->type!=string||token->type!=numberFloat||token->type!=singleChars))
    {
        return NULL;
    }
    
    while (token->type==identifier||token->type == number||token->type==string||token->type==operation||token->type==singleChars||token->type==numberFloat)
    {
        if (token->type==unknown)
        {
            break;
        }
        expression_value *value = expression_value_create(token);
        if (token->lexeme[0] == '(')
        {
            openBrackets++;
        }
        else if (token->lexeme[0] == ')')
        {
            openBrackets--;
        }
        if (openBrackets < 0)
        {
            break;
        }
        
        expression_value *last = expression_last(stack);
        int action = precTable[last->index][value->index];
        printf("%s\n",value->value);
        if (action == S)
        {
            last->action = S;
            expression_list_insert(stack, value);
            token = new_token(fp, token);
        }
        else if (action == R)
        {
            reduce(stack);
        }
        else if (action == Err)
        {
            printf("Error: syntax error\n");
            free(value);
            break;
        }
        else if (action == Eq)
        {
            expression_element *tmp = stack->head;
            while (tmp->next!=NULL)
            {
                tmp=tmp->next;
            }
            tmp->value = expression_list_pop(stack);
            disposeValue(expression_list_pop(stack));
            expression_list_insert(stack, tmp->value);
            free(value);
            token = new_token(fp, token);
        }
        else if (action == Fin)
        {
            printf("Konec\n");
            break;
        }
        else
        {
            printf("Error: syntax error\n");
            free(value);
            break;
        }
        
        if (token->lexeme[0] == EOF){break;}

    }
                if (stack->head->next!=NULL)
            {
                while (stack->head->next->next != NULL)
            {
                reduce(stack);
            }
            }
            retVal = expression_list_pop(stack);
    expression_list_dispose(stack);
    // destroyToken(token);
    // fclose(fp);
    // printf("Hodnota lexemu: %s \n",token->lexeme);
    return retVal;
}

