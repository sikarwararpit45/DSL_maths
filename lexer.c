#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"

static void advance(Lexer* lexer)
{
    lexer->pos++;
    lexer->current = lexer->src[lexer->pos];
}

void lexer_init(Lexer* lexer, const char* src)
{
    lexer->src = src;
    lexer->pos = 0;
    lexer->current = src[0];
}

static void skip_whitespace(Lexer* lexer)
{
    while (isspace(lexer->current))
    {
        advance(lexer);
    }
}

static Token make_token(TokenType type, const char* text)
{
    Token token;

    token.type = type;

    strcpy(token.text, text);

    token.value = 0;

    return token;
}

static Token number(Lexer* lexer)
{
    char buffer[64];

    int i = 0;

    while (isdigit(lexer->current) || lexer->current == '.')
    {
        buffer[i++] = lexer->current;

        advance(lexer);
    }

    buffer[i] = '\0';

    Token token;

    token.type = TOKEN_NUMBER;

    strcpy(token.text, buffer);

    token.value = atof(buffer);

    return token;
}

static Token identifier(Lexer* lexer)
{
    char buffer[64];

    int i = 0;

    while (isalnum(lexer->current) || lexer->current == '_')
    {
        buffer[i++] = lexer->current;

        advance(lexer);
    }

    buffer[i] = '\0';

    if (strcmp(buffer, "let") == 0)
        return make_token(TOKEN_LET, buffer);

    if (strcmp(buffer, "print") == 0)
        return make_token(TOKEN_PRINT, buffer);

    if (strcmp(buffer, "if") == 0)
        return make_token(TOKEN_IF, buffer);

    if (strcmp(buffer, "else") == 0)
        return make_token(TOKEN_ELSE, buffer);

    if (strcmp(buffer, "while") == 0)
        return make_token(TOKEN_WHILE, buffer);

    return make_token(TOKEN_IDENTIFIER, buffer);
}

Token lexer_next_token(Lexer* lexer)
{
    while (lexer->current != '\0')
    {
        if (isspace(lexer->current))
        {
            skip_whitespace(lexer);
            continue;
        }

        if (isdigit(lexer->current))
        {
            return number(lexer);
        }

        if (isalpha(lexer->current))
        {
            return identifier(lexer);
        }

        switch (lexer->current)
        {
            case '+':
                advance(lexer);
                return make_token(TOKEN_PLUS, "+");

            case '-':
                advance(lexer);
                return make_token(TOKEN_MINUS, "-");

            case '*':
                advance(lexer);
                return make_token(TOKEN_STAR, "*");

            case '/':
                advance(lexer);
                return make_token(TOKEN_SLASH, "/");

            case '=':
                advance(lexer);

                if (lexer->current == '=')
                {
                    advance(lexer);
                    return make_token(TOKEN_EQUAL_EQUAL, "==");
                }

                return make_token(TOKEN_EQUAL, "=");

            case '>':
                advance(lexer);
                return make_token(TOKEN_GREATER, ">");

            case '<':
                advance(lexer);
                return make_token(TOKEN_LESS, "<");

            case '(':
                advance(lexer);
                return make_token(TOKEN_LPAREN, "(");

            case ')':
                advance(lexer);
                return make_token(TOKEN_RPAREN, ")");

            case '{':
                advance(lexer);
                return make_token(TOKEN_LBRACE, "{");

            case '}':
                advance(lexer);
                return make_token(TOKEN_RBRACE, "}");

            case ';':
                advance(lexer);
                return make_token(TOKEN_SEMICOLON, ";");

            case ',':
                advance(lexer);
                return make_token(TOKEN_COMMA, ",");

            default:
                printf("Unknown character: %c\n", lexer->current);
                exit(1);
        }
    }

    return make_token(TOKEN_EOF, "EOF");
}