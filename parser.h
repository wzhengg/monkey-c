#pragma once

#include "ast.h"
#include "lexer.h"
#include "token.h"

typedef enum {
	PRECEDENCE_LOWEST = 1,
	PRECEDENCE_EQUALS,
	PRECEDENCE_LESSGREATER,
	PRECEDENCE_SUM,
	PRECEDENCE_PRODUCT,
	PRECEDENCE_PREFIX,
	PRECEDENCE_CALL,
} op_precedence_t;

typedef struct parser parser_t;

typedef ast_node_t *(*prefix_parse_fn)(parser_t *parser);
typedef ast_node_t *(*infix_parse_fn)(parser_t *parser, ast_node_t *);

typedef struct parser {
	lexer_t *lexer;
	token_t *cur_token;
	token_t *peek_token;

	prefix_parse_fn *prefix_parse_fns;
	infix_parse_fn *infix_parse_fns;
} parser_t;

parser_t *parser_new(lexer_t *lexer);
ast_node_t *parse_program(parser_t *parser);
