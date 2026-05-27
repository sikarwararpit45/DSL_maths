#include <string.h>
#include <stdio.h>

#include "compiler.h"

typedef struct {
    char name[64];
    int slot;
} Symbol;

static Symbol symbols[256];

static int symbol_count = 0;

static void emit(Bytecode* bc, int value)
{
    bc->code[bc->code_size++] = value;
}

static int add_constant(Bytecode* bc, double value)
{
    int index = bc->const_size;

    bc->constants[index] = value;

    bc->const_size++;

    return index;
}

static int get_symbol(const char* name)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbols[i].name, name) == 0)
        {
            return symbols[i].slot;
        }
    }

    strcpy(symbols[symbol_count].name, name);

    symbols[symbol_count].slot = symbol_count;

    symbol_count++;

    return symbol_count - 1;
}

void compile(ASTNode* node, Bytecode* bc)
{
    while (node)
    {
        switch (node->type)
        {
            case AST_NUMBER:
            {
                int index = add_constant(bc, node->number);

                emit(bc, OP_PUSH);
                emit(bc, index);

                break;
            }

            case AST_VARIABLE:
            {
                int slot = get_symbol(node->name);

                emit(bc, OP_LOAD);
                emit(bc, slot);

                break;
            }

            case AST_ASSIGN:
            {
                compile(node->left, bc);

                int slot = get_symbol(node->name);

                emit(bc, OP_STORE);
                emit(bc, slot);

                break;
            }

            case AST_BINARY:
            {
                compile(node->left, bc);

                compile(node->right, bc);

                switch (node->op)
                {
                    case '+':
                        emit(bc, OP_ADD);
                        break;

                    case '-':
                        emit(bc, OP_SUB);
                        break;

                    case '*':
                        emit(bc, OP_MUL);
                        break;

                    case '/':
                        emit(bc, OP_DIV);
                        break;
                }

                break;
            }

            case AST_PRINT:
            {
                compile(node->left, bc);

                emit(bc, OP_PRINT);

                break;
            }

            case AST_FUNCTION_CALL:
            {
                compile(node->arg1, bc);

                if (strcmp(node->name, "sin") == 0)
                {
                    emit(bc, OP_SIN);
                }
                else if (strcmp(node->name, "cos") == 0)
                {
                    emit(bc, OP_COS);
                }
                else if (strcmp(node->name, "tan") == 0)
                {
                    emit(bc, OP_TAN);
                }
                else if (strcmp(node->name, "sqrt") == 0)
                {
                    emit(bc, OP_SQRT);
                }
                else if (strcmp(node->name, "pow") == 0)
                {
                    compile(node->arg2, bc);

                    emit(bc, OP_POW);
                }

                break;
            }
        }

        node = node->next;
    }
}