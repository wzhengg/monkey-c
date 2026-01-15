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

typedef ast_expr_t *(*parser_prefix_parse_fn)(parser_t *parser);
typedef ast_expr_t *(*parser_infix_parse_fn)(parser_t *parser, ast_expr_t *);

typedef struct parser {
	lexer_t *lexer;
	token_t *cur_token;
	token_t *peek_token;

	parser_prefix_parse_fn *prefix_parse_fns;
	parser_infix_parse_fn *infix_parse_fns;
} parser_t;

parser_t *parser_new(lexer_t *lexer);
ast_node_t *parser_parse_program(parser_t *parser);
