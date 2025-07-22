#include "vbc.h"

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
}

/* int accept(char **s, char c)
{
    if (**s)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}
 */
int check_parentheses(char *s)
{
    int balance = 0;
    int i = -1;

    if (!s)
        return (-1);
    while(s[++i])
    {
        if (s[i] == '(')
            balance++;
        else if (s[i] == ')')
        {
            balance--;
            if (balance < 0)
                return (-1);
        }
    }
    return (balance);
}
//...

node    *parse_nbr_or_group(char **s)
{
    node    *res = NULL;
    node    temp;

    if (**s == '(')
    {
        (*s)++;
        res = parse_add(s);
        if (!res || **s != ')')
        {
            destroy_tree(res);
            unexpected(**s);
            return (NULL);
        }
        (*s)++;
        return (res);
    }
    if (isdigit(**s))
    {
        temp.type = VAL;
        temp.val = **s - '0';
        res = new_node(temp);
        if (!res)
            return (NULL);
        (*s)++;
        return (res);
    }
    unexpected(**s);
    return (NULL);
}

node    *parse_mult(char **s)
{
    node    *l;
    node    *r;
    node    temp;

    l = parse_nbr_or_group(s);
    if (!l)
        return (NULL);
    while (**s == '*')
    {
        (*s)++;
        r = parse_nbr_or_group(s);
        if (!r)
        {
            destroy_tree(l);
            return (NULL);
        }
        temp.type = MULTI;
        temp.l = l;
        temp.r = r;
        l = new_node(temp);
    }
    return (l);
}

node    *parse_add(char **s)
{
    node    *l;
    node    *r;
    node    temp;

    l = parse_mult(s);
    if (!l)
        return (NULL);
    while (**s == '+')
    {
        (*s)++;
        r = parse_mult(s);
        if (!r)
        {
            destroy_tree(l);
            return (NULL);
        }
        temp.type = ADD;
        temp.l = l;
        temp.r = r;
        l = new_node(temp);
    }
    return (l);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int main(int argc, char **argv)
{
    node *tree;
    char *input = argv[1];

    if (argc != 2)
        return (1);
    if (check_parentheses(argv[1]) == -1)
    {
        printf("Unexpected token ')'\n");
        return (1);
    }
    else if (check_parentheses(argv[1]) != 0)
    {
        printf("Unexpected token '('\n");
        return (1);       
    } 
    tree = parse_add(&input);
    if (!tree)
        return (1);
    if (*input)
    {
        unexpected(*input);
        destroy_tree(tree);
        return (1);
    }    
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
