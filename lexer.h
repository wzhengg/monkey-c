#pragma once

#include "token.h"
#include <stddef.h>

typedef struct {
	char *input;
	size_t n;
	int position;
	int read_position;
	char ch;
} lexer_t;

lexer_t *lexer_new(char *input);
token_t *lexer_next_token(lexer_t *l);
