#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer lexer;
    Token current;
} Parser;

void parser_init(Parser* parser, const char* src);

ASTNode* parse_program(Parser* parser);

#endif