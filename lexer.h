#ifndef LEXER_H
#define LEXER_H

#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_EOF,

    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,

    TOKEN_LET,
    TOKEN_PRINT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,

    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,

    TOKEN_GREATER,
    TOKEN_LESS,

    TOKEN_LPAREN,
    TOKEN_RPAREN,

    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_SEMICOLON,
    TOKEN_COMMA
} TokenType;

typedef struct {
    TokenType type;
    char text[MAX_TOKEN_LEN];
    double value;
} Token;

typedef struct {
    const char* src;
    int pos;
    char current;
} Lexer;

void lexer_init(Lexer* lexer, const char* src);

Token lexer_next_token(Lexer* lexer);

#endif