#ifndef AST_H
#define AST_H

typedef enum {
    AST_NUMBER,
    AST_VARIABLE,
    AST_BINARY,

    AST_ASSIGN,
    AST_PRINT,

    AST_IF,
    AST_WHILE,

    AST_BLOCK,

    AST_FUNCTION_CALL
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;

    double number;

    char name[64];

    char op;

    struct ASTNode* left;
    struct ASTNode* right;

    struct ASTNode* condition;

    struct ASTNode* body;

    struct ASTNode* else_body;

    struct ASTNode* next;

    struct ASTNode* arg1;
    struct ASTNode* arg2;

} ASTNode;

#endif