#include "parser.h"

#include "lexer.h"
#include "list.h"
#include "token.h"

#include <stddef.h>
#include <stdlib.h>

static ast_stmt_t *parse_statement(parser_t *parser);
static ast_stmt_t *parse_let_statement(parser_t *parser);
static ast_stmt_t *parse_return_statement(parser_t *parser);
static void next_token(parser_t *parser);
static bool cur_token_is(parser_t *parser, token_type_t type);
static bool peek_token_is(parser_t *parser, token_type_t type);
static bool expect_peek(parser_t *parser, token_type_t type);

parser_t *parser_new(lexer_t *lexer) {
	parser_t *parser = malloc(sizeof(parser_t));
	if (parser == NULL) {
		return NULL;
	}

	parser->lexer = lexer;
	parser->cur_token = NULL;
	parser->peek_token = NULL;

	next_token(parser);
	next_token(parser);

	return parser;
}

ast_node_t *parser_parse_program(parser_t *parser) {
	ast_node_t *node = malloc(sizeof(ast_node_t));
	if (node == NULL) {
		return NULL;
	}

	ast_prog_t *prog = malloc(sizeof(ast_prog_t));
	if (prog == NULL) {
		free(node);
		return NULL;
	}

	prog->stmts = list_new(64);
	if (prog->stmts == NULL) {
		free(node);
		free(prog);
		return NULL;
	}

	node->type = AST_PROGRAM;
	node->data.prog = prog;

	while (!cur_token_is(parser, TOKEN_EOF)) {
		ast_stmt_t *stmt = parse_statement(parser);
		if (stmt != NULL) {
			ast_prog_stmts_append(prog, stmt);
		}
		next_token(parser);
	}

	return node;
}

static ast_stmt_t *parse_statement(parser_t *parser) {
	switch (parser->cur_token->type) {
		case TOKEN_LET:
			return parse_let_statement(parser);
		case TOKEN_RETURN:
			return parse_return_statement(parser);
		default:
			return NULL;
	}
}

static ast_stmt_t *parse_let_statement(parser_t *parser) {
	ast_stmt_t *stmt = malloc(sizeof(ast_stmt_t));
	if (stmt == NULL) {
		return NULL;
	}

	stmt->type = AST_LET;

	stmt->data.let = malloc(sizeof(ast_let_stmt_t));
	if (stmt->data.let == NULL) {
		free(stmt);
		return NULL;
	}

	stmt->data.let->token = parser->cur_token;

	if (!expect_peek(parser, TOKEN_IDENT)) {
		free(stmt->data.let);
		free(stmt);
		return NULL;
	}

	stmt->data.let->name = malloc(sizeof(ast_ident_expr_t));
	if (stmt->data.let->name == NULL) {
		free(stmt->data.let);
		free(stmt);
		return NULL;
	}

	stmt->data.let->name->token = parser->cur_token;
	stmt->data.let->name->value = parser->cur_token->literal;

	if (!expect_peek(parser, TOKEN_ASSIGN)) {
		free(stmt->data.let->name);
		free(stmt->data.let);
		free(stmt);
		return NULL;
	}

	while (!cur_token_is(parser, TOKEN_SEMICOLON)) {
		next_token(parser);
	}

	return stmt;
}

static ast_stmt_t *parse_return_statement(parser_t *parser) {
	ast_stmt_t *stmt = malloc(sizeof(ast_stmt_t));
	if (stmt == NULL) {
		return NULL;
	}

	stmt->type = AST_RETURN;

	stmt->data.ret = malloc(sizeof(ast_return_stmt_t));
	if (stmt->data.ret == NULL) {
		free(stmt);
		return NULL;
	}

	stmt->data.ret->token = parser->cur_token;

	next_token(parser);

	while (!cur_token_is(parser, TOKEN_SEMICOLON)) {
		next_token(parser);
	}

	return stmt;
}

static void next_token(parser_t *parser) {
	parser->cur_token = parser->peek_token;
	parser->peek_token = lexer_next_token(parser->lexer);
}

static bool cur_token_is(parser_t *parser, token_type_t type) {
	return parser->cur_token->type == type;
}

static bool peek_token_is(parser_t *parser, token_type_t type) {
	return parser->peek_token->type == type;
}

static bool expect_peek(parser_t *parser, token_type_t type) {
	if (peek_token_is(parser, type)) {
		next_token(parser);
		return true;
	}
	return false;
}
