#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static ASTNode* create_node(ASTNodeType type)
{
    ASTNode* node = malloc(sizeof(ASTNode));

    memset(node, 0, sizeof(ASTNode));

    node->type = type;

    return node;
}

void parser_init(Parser* parser, const char* src)
{
    lexer_init(&parser->lexer, src);

    parser->current = lexer_next_token(&parser->lexer);
}

static void eat(Parser* parser, TokenType type)
{
    if (parser->current.type == type)
    {
        parser->current = lexer_next_token(&parser->lexer);
    }
    else
    {
        printf("\nParser Error\n");
        printf("Unexpected token: %s\n", parser->current.text);
        printf("Expected token type: %d\n", type);

        exit(1);
    }
}

static ASTNode* expression(Parser* parser);

static ASTNode* factor(Parser* parser)
{
    Token token = parser->current;

    if (token.type == TOKEN_NUMBER)
    {
        ASTNode* node = create_node(AST_NUMBER);

        node->number = token.value;

        eat(parser, TOKEN_NUMBER);

        return node;
    }

    if (token.type == TOKEN_IDENTIFIER)
    {
        char name[64];

        strcpy(name, token.text);

        eat(parser, TOKEN_IDENTIFIER);

        if (parser->current.type == TOKEN_LPAREN)
        {
            eat(parser, TOKEN_LPAREN);

            ASTNode* node = create_node(AST_FUNCTION_CALL);

            strcpy(node->name, name);

            node->arg1 = expression(parser);

            if (parser->current.type == TOKEN_COMMA)
            {
                eat(parser, TOKEN_COMMA);

                node->arg2 = expression(parser);
            }

            eat(parser, TOKEN_RPAREN);

            return node;
        }

        ASTNode* node = create_node(AST_VARIABLE);

        strcpy(node->name, name);

        return node;
    }

    if (token.type == TOKEN_LPAREN)
    {
        eat(parser, TOKEN_LPAREN);

        ASTNode* node = expression(parser);

        eat(parser, TOKEN_RPAREN);

        return node;
    }

    printf("Invalid expression near: %s\n", parser->current.text);

    exit(1);
}

static ASTNode* term(Parser* parser)
{
    ASTNode* node = factor(parser);

    while (parser->current.type == TOKEN_STAR ||
           parser->current.type == TOKEN_SLASH)
    {
        ASTNode* bin = create_node(AST_BINARY);

        bin->left = node;

        if (parser->current.type == TOKEN_STAR)
        {
            bin->op = '*';

            eat(parser, TOKEN_STAR);
        }
        else
        {
            bin->op = '/';

            eat(parser, TOKEN_SLASH);
        }

        bin->right = factor(parser);

        node = bin;
    }

    return node;
}

static ASTNode* expression(Parser* parser)
{
    ASTNode* node = term(parser);

    while (parser->current.type == TOKEN_PLUS ||
           parser->current.type == TOKEN_MINUS)
    {
        ASTNode* bin = create_node(AST_BINARY);

        bin->left = node;

        if (parser->current.type == TOKEN_PLUS)
        {
            bin->op = '+';

            eat(parser, TOKEN_PLUS);
        }
        else
        {
            bin->op = '-';

            eat(parser, TOKEN_MINUS);
        }

        bin->right = term(parser);

        node = bin;
    }

    return node;
}

static ASTNode* statement(Parser* parser)
{
    if (parser->current.type == TOKEN_PRINT)
    {
        eat(parser, TOKEN_PRINT);

        ASTNode* node = create_node(AST_PRINT);

        node->left = expression(parser);

        eat(parser, TOKEN_SEMICOLON);

        return node;
    }

    if (parser->current.type == TOKEN_LET)
    {
        eat(parser, TOKEN_LET);

        ASTNode* node = create_node(AST_ASSIGN);

        strcpy(node->name, parser->current.text);

        eat(parser, TOKEN_IDENTIFIER);

        eat(parser, TOKEN_EQUAL);

        node->left = expression(parser);

        eat(parser, TOKEN_SEMICOLON);

        return node;
    }

    return expression(parser);
}

ASTNode* parse_program(Parser* parser)
{
    ASTNode* head = NULL;

    ASTNode* current = NULL;

    while (parser->current.type != TOKEN_EOF)
    {
        ASTNode* stmt = statement(parser);

        if (!head)
        {
            head = stmt;
            current = stmt;
        }
        else
        {
            current->next = stmt;
            current = stmt;
        }
    }

    return head;
}