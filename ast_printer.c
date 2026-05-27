#include <stdio.h>

#include "ast_printer.h"

static void indent(int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }
}

void print_ast(ASTNode* node, int level)
{
    while (node)
    {
        indent(level);

        switch (node->type)
        {
            case AST_NUMBER:
                printf("NUMBER(%lf)\n", node->number);
                break;

            case AST_VARIABLE:
                printf("VARIABLE(%s)\n", node->name);
                break;

            case AST_ASSIGN:
                printf("ASSIGN(%s)\n", node->name);
                print_ast(node->left, level + 1);
                break;

            case AST_PRINT:
                printf("PRINT\n");
                print_ast(node->left, level + 1);
                break;
             case AST_BINARY:
                printf("BINARY(%c)\n", node->op);

                print_ast(node->left, level + 1);
                print_ast(node->right, level + 1);

                break;

            case AST_FUNCTION_CALL:
                printf("FUNCTION(%s)\n", node->name);

                print_ast(node->arg1, level + 1);

                if (node->arg2)
                {
                    print_ast(node->arg2, level + 1);
                }

                break;

            default:
                printf("UNKNOWN NODE\n");
                break;
        }

        node = node->next;
    }
}