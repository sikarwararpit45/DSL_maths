#ifndef COMPILER_H
#define COMPILER_H

#include "ast.h"

#define MAX_CODE 1024
#define MAX_CONSTANTS 1024

typedef enum {
    OP_PUSH,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    OP_STORE,
    OP_LOAD,

    OP_PRINT,

    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_SQRT,
    OP_POW,

    OP_HALT
} OpCode;

typedef struct {
    int code[MAX_CODE];

    double constants[MAX_CONSTANTS];

    int code_size;

    int const_size;
} Bytecode;

void compile(ASTNode* node, Bytecode* bc);

#endif