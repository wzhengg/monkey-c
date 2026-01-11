#pragma once

#include "ast.h"
#include "lexer.h"
#include "token.h"

typedef struct {
	lexer_t *lexer;
	token_t *cur_token;
	token_t *peek_token;
} parser_t;

parser_t *parser_new(lexer_t *lexer);
ast_node_t *parser_parse_program(parser_t *parser);
