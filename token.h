#pragma once

typedef enum {
	TOKEN_ILLEGAL,
	TOKEN_EOF,

	TOKEN_IDENT,
	TOKEN_INT,

	TOKEN_ASSIGN,
	TOKEN_PLUS,

	TOKEN_COMMA,
	TOKEN_SEMICOLON,

	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACE,
	TOKEN_RBRACE,

	TOKEN_FUNCTION,
	TOKEN_LET,
} token_type_t;

typedef struct {
	token_type_t type;
	char *literal;
} token_t;

token_type_t lookup_ident(char *ident);
