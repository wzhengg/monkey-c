#include "parser.h"

#include "lexer.h"
#include "list.h"
#include "token.h"

#include <stddef.h>
#include <stdlib.h>

op_precedence_t precedences[TOKEN_COUNT] = {
	[TOKEN_EQ] = PRECEDENCE_EQUALS,
	[TOKEN_NEQ] = PRECEDENCE_EQUALS,
	[TOKEN_LT] = PRECEDENCE_LESSGREATER,
	[TOKEN_GT] = PRECEDENCE_LESSGREATER,
	[TOKEN_PLUS] = PRECEDENCE_SUM,
	[TOKEN_MINUS] = PRECEDENCE_SUM,
	[TOKEN_SLASH] = PRECEDENCE_PRODUCT,
	[TOKEN_ASTERISK] = PRECEDENCE_PRODUCT
};

static ast_stmt_t *parse_statement(parser_t *parser);
static ast_stmt_t *parse_expression_statement(parser_t *parser);
static ast_stmt_t *parse_let_statement(parser_t *parser);
static ast_stmt_t *parse_return_statement(parser_t *parser);

static ast_expr_t *parse_expression(parser_t *parser, op_precedence_t precedence);
static ast_expr_t *parse_boolean(parser_t *parser);
static ast_expr_t *parse_identifier(parser_t *parser);
static ast_expr_t *parse_integer_literal(parser_t *parser);
static ast_expr_t *parse_prefix_expression(parser_t *parser);
static ast_expr_t *parse_infix_expression(parser_t *parser, ast_expr_t *left);
static ast_expr_t *parse_grouped_expression(parser_t *parser);
static ast_expr_t *ast_expr_new(ast_expr_type_t type);

static void next_token(parser_t *parser);
static bool cur_token_is(parser_t *parser, token_type_t type);
static bool peek_token_is(parser_t *parser, token_type_t type);
static bool expect_peek(parser_t *parser, token_type_t type);

static void register_prefix(parser_t *parser, token_type_t type, parser_prefix_parse_fn fn);
static void register_infix(parser_t *parser, token_type_t type, parser_infix_parse_fn fn);

static op_precedence_t peek_precedence(parser_t *parser);
static op_precedence_t cur_precedence(parser_t *parser);

parser_t *parser_new(lexer_t *lexer) {
	parser_t *parser = malloc(sizeof(parser_t));
	if (parser == NULL) {
		return NULL;
	}

	parser->lexer = lexer;
	parser->cur_token = NULL;
	parser->peek_token = NULL;

	parser->prefix_parse_fns = calloc(TOKEN_COUNT, sizeof(parser_prefix_parse_fn));
	if (parser->prefix_parse_fns == NULL) {
		free(parser);
		return NULL;
	}

	parser->infix_parse_fns = calloc(TOKEN_COUNT, sizeof(parser_infix_parse_fn));
	if (parser->infix_parse_fns == NULL) {
		free(parser->prefix_parse_fns);
		free(parser);
		return NULL;
	}

	register_prefix(parser, TOKEN_TRUE, parse_boolean);
	register_prefix(parser, TOKEN_FALSE, parse_boolean);
	register_prefix(parser, TOKEN_IDENT, parse_identifier);
	register_prefix(parser, TOKEN_INT, parse_integer_literal);
	register_prefix(parser, TOKEN_BANG, parse_prefix_expression);
	register_prefix(parser, TOKEN_MINUS, parse_prefix_expression);
	register_prefix(parser, TOKEN_LPAREN, parse_grouped_expression);

	register_infix(parser, TOKEN_PLUS, parse_infix_expression);
	register_infix(parser, TOKEN_MINUS, parse_infix_expression);
	register_infix(parser, TOKEN_SLASH, parse_infix_expression);
	register_infix(parser, TOKEN_ASTERISK, parse_infix_expression);
	register_infix(parser, TOKEN_EQ, parse_infix_expression);
	register_infix(parser, TOKEN_NEQ, parse_infix_expression);
	register_infix(parser, TOKEN_LT, parse_infix_expression);
	register_infix(parser, TOKEN_GT, parse_infix_expression);

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
			return parse_expression_statement(parser);
	}
}

static ast_stmt_t *parse_expression_statement(parser_t *parser) {
	ast_stmt_t *stmt = malloc(sizeof(ast_stmt_t));
	if (stmt == NULL) {
		return NULL;
	}

	stmt->type = AST_EXPR_STMT;

	stmt->data.expr = malloc(sizeof(ast_expr_stmt_t));
	if (stmt->data.expr == NULL) {
		free(stmt);
		return NULL;
	}

	stmt->data.expr->token = parser->cur_token;

	stmt->data.expr->expr = parse_expression(parser, PRECEDENCE_LOWEST);

	if (peek_token_is(parser, TOKEN_SEMICOLON)) {
		next_token(parser);
	}

	return stmt;
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

static ast_expr_t *parse_expression(parser_t *parser, op_precedence_t precedence) {
	parser_prefix_parse_fn prefix = parser->prefix_parse_fns[parser->cur_token->type];
	if (prefix == NULL) {
		return NULL;
	}

	ast_expr_t *left_expr = prefix(parser);

	while (!peek_token_is(parser, TOKEN_SEMICOLON) && precedence < peek_precedence(parser)) {
		parser_infix_parse_fn infix = parser->infix_parse_fns[parser->peek_token->type];
		if (infix == NULL) {
			return left_expr;
		}

		next_token(parser);

		left_expr = infix(parser, left_expr);
	}

	return left_expr;
}

static ast_expr_t *parse_boolean(parser_t *parser) {
	ast_expr_t *expr = ast_expr_new(AST_BOOLEAN);
	if (expr == NULL) {
		return NULL;
	}

	expr->data.boolean = malloc(sizeof(ast_bool_expr_t));
	if (expr->data.boolean == NULL) {
		free(expr);
		return NULL;
	}

	expr->data.boolean->token = parser->cur_token;
	expr->data.boolean->value = cur_token_is(parser, TOKEN_TRUE);

	return expr;
}

static ast_expr_t *parse_identifier(parser_t *parser) {
	ast_expr_t *expr = ast_expr_new(AST_IDENTIFIER);
	if (expr == NULL) {
		return NULL;
	}

	expr->data.ident = malloc(sizeof(ast_ident_expr_t));
	if (expr->data.ident == NULL) {
		free(expr);
		return NULL;
	}

	expr->data.ident->token = parser->cur_token;
	expr->data.ident->value = parser->cur_token->literal;

	return expr;
}

static ast_expr_t *parse_integer_literal(parser_t *parser) {
	ast_expr_t *expr = ast_expr_new(AST_INT_LITERAL);
	if (expr == NULL) {
		return expr;
	}

	expr->data.int_lit = malloc(sizeof(ast_int_lit_expr_t));
	if (expr->data.int_lit == NULL) {
		free(expr);
		return NULL;
	}

	expr->data.int_lit->token = parser->cur_token;
	expr->data.int_lit->value = strtoll(parser->cur_token->literal, NULL, 10);

	return expr;
}

static ast_expr_t *parse_prefix_expression(parser_t *parser) {
	ast_expr_t *expr = ast_expr_new(AST_PREFIX);
	if (expr == NULL) {
		return NULL;
	}

	expr->data.prefix = malloc(sizeof(ast_prefix_expr_t));
	if (expr->data.prefix == NULL) {
		free(expr);
		return NULL;
	}

	expr->data.prefix->token = parser->cur_token;
	expr->data.prefix->op = parser->cur_token->literal;

	next_token(parser);

	expr->data.prefix->right = parse_expression(parser, PRECEDENCE_PREFIX);

	return expr;
}

static ast_expr_t *parse_infix_expression(parser_t *parser, ast_expr_t *left) {
	ast_expr_t *expr = ast_expr_new(AST_INFIX);
	if (expr == NULL) {
		return NULL;
	}

	expr->data.infix = malloc(sizeof(ast_infix_expr_t));
	if (expr->data.infix == NULL) {
		free(expr);
		return NULL;
	}

	expr->data.infix->token = parser->cur_token;
	expr->data.infix->op = parser->cur_token->literal;
	expr->data.infix->left = left;

	op_precedence_t precedence = cur_precedence(parser);
	next_token(parser);
	expr->data.infix->right = parse_expression(parser, precedence);

	return expr;
}

static ast_expr_t *parse_grouped_expression(parser_t *parser) {
	next_token(parser);

	ast_expr_t *expr = parse_expression(parser, PRECEDENCE_LOWEST);

	if (!peek_token_is(parser, TOKEN_RPAREN)) {
		return NULL;
	}

	return expr;
}

static ast_expr_t *ast_expr_new(ast_expr_type_t type) {
	ast_expr_t *expr = malloc(sizeof(ast_expr_t));
	if (expr == NULL) {
		return NULL;
	}

	expr->type = type;

	return expr;
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

static void register_prefix(parser_t *parser, token_type_t type, parser_prefix_parse_fn fn) {
	parser->prefix_parse_fns[type] = fn;
}

static void register_infix(parser_t *parser, token_type_t type, parser_infix_parse_fn fn) {
	parser->infix_parse_fns[type] = fn;
}

static op_precedence_t peek_precedence(parser_t *parser) {
	if (precedences[parser->peek_token->type] != 0) {
		return precedences[parser->peek_token->type];
	}

	return PRECEDENCE_LOWEST;
}

static op_precedence_t cur_precedence(parser_t *parser) {
	if (precedences[parser->cur_token->type] != 0) {
		return precedences[parser->cur_token->type];
	}

	return PRECEDENCE_LOWEST;
}
