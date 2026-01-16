#include "parser.h"

#include "lexer.h"
#include "list.h"
#include "token.h"

#include <stddef.h>
#include <stdlib.h>

static op_precedence_t precedences[TOKEN_COUNT] = {
	[TOKEN_EQ] = PRECEDENCE_EQUALS,
	[TOKEN_NEQ] = PRECEDENCE_EQUALS,
	[TOKEN_LT] = PRECEDENCE_LESSGREATER,
	[TOKEN_GT] = PRECEDENCE_LESSGREATER,
	[TOKEN_PLUS] = PRECEDENCE_SUM,
	[TOKEN_MINUS] = PRECEDENCE_SUM,
	[TOKEN_SLASH] = PRECEDENCE_PRODUCT,
	[TOKEN_ASTERISK] = PRECEDENCE_PRODUCT,
	[TOKEN_LPAREN] = PRECEDENCE_CALL
};

static ast_node_t *parse_statement(parser_t *parser);
static ast_node_t *parse_block_statement(parser_t *parser);
static ast_node_t *parse_expression_statement(parser_t *parser);
static ast_node_t *parse_let_statement(parser_t *parser);
static ast_node_t *parse_return_statement(parser_t *parser);

static ast_node_t *parse_expression(parser_t *parser, op_precedence_t precedence);
static ast_node_t *parse_boolean(parser_t *parser);
static ast_node_t *parse_call_expression(parser_t *parser, ast_node_t *function);
static ast_node_t *parse_function_literal(parser_t *parser);
static ast_node_t *parse_grouped_expression(parser_t *parser);
static ast_node_t *parse_identifier(parser_t *parser);
static ast_node_t *parse_if_expression(parser_t *parser);
static ast_node_t *parse_infix_expression(parser_t *parser, ast_node_t *left);
static ast_node_t *parse_integer_literal(parser_t *parser);
static ast_node_t *parse_prefix_expression(parser_t *parser);
static list_t *parse_call_arguments(parser_t *parser);
static list_t *parse_function_parameters(parser_t *parser);

static void next_token(parser_t *parser);
static bool cur_token_is(parser_t *parser, token_type_t type);
static bool peek_token_is(parser_t *parser, token_type_t type);
static bool expect_peek(parser_t *parser, token_type_t type);

static void register_prefix(parser_t *parser, token_type_t type, prefix_parse_fn fn);
static void register_infix(parser_t *parser, token_type_t type, infix_parse_fn fn);

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

	parser->prefix_parse_fns = calloc(TOKEN_COUNT, sizeof(prefix_parse_fn));
	if (parser->prefix_parse_fns == NULL) {
		free(parser);
		return NULL;
	}

	parser->infix_parse_fns = calloc(TOKEN_COUNT, sizeof(infix_parse_fn));
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
	register_prefix(parser, TOKEN_IF, parse_if_expression);
	register_prefix(parser, TOKEN_FUNCTION, parse_function_literal);

	register_infix(parser, TOKEN_PLUS, parse_infix_expression);
	register_infix(parser, TOKEN_MINUS, parse_infix_expression);
	register_infix(parser, TOKEN_SLASH, parse_infix_expression);
	register_infix(parser, TOKEN_ASTERISK, parse_infix_expression);
	register_infix(parser, TOKEN_EQ, parse_infix_expression);
	register_infix(parser, TOKEN_NEQ, parse_infix_expression);
	register_infix(parser, TOKEN_LT, parse_infix_expression);
	register_infix(parser, TOKEN_GT, parse_infix_expression);
	register_infix(parser, TOKEN_LPAREN, parse_call_expression);

	next_token(parser);
	next_token(parser);

	return parser;
}

ast_node_t *parse_program(parser_t *parser) {
	ast_node_t *node = ast_node_prog_new(64);
	if (node == NULL) {
		exit(1);
	}

	while (!cur_token_is(parser, TOKEN_EOF)) {
		ast_node_t *stmt = parse_statement(parser);
		if (stmt != NULL) {
			ast_prog_stmts_append(node->data.prog, stmt);
		}
		next_token(parser);
	}

	return node;
}

static ast_node_t *parse_statement(parser_t *parser) {
	switch (parser->cur_token->type) {
		case TOKEN_LET:
			return parse_let_statement(parser);
		case TOKEN_RETURN:
			return parse_return_statement(parser);
		default:
			return parse_expression_statement(parser);
	}
}

static ast_node_t *parse_block_statement(parser_t *parser) {
	ast_node_t *node = ast_node_stmt_new(AST_STMT_BLOCK);
	if (node == NULL) {
		exit(1);
	}

	node->data.stmt->data.block->token = parser->cur_token;
	node->data.stmt->data.block->stmts = list_new(64); // TODO
	if (node->data.stmt->data.block->stmts == NULL) {
		exit(1);
	}

	next_token(parser);

	while (!cur_token_is(parser, TOKEN_RBRACE) && !cur_token_is(parser, TOKEN_EOF)) {
		ast_node_t *stmt = parse_statement(parser);
		if (stmt != NULL) {
			list_append(node->data.stmt->data.block->stmts, stmt); // TODO
		}
		next_token(parser);
	}

	return node;
}

static ast_node_t *parse_expression_statement(parser_t *parser) {
	ast_node_t *node = ast_node_stmt_new(AST_STMT_EXPR);
	if (node == NULL) {
		exit(1);
	}

	node->data.stmt->data.expr->token = parser->cur_token;
	node->data.stmt->data.expr->expr = parse_expression(parser, PRECEDENCE_LOWEST);

	if (peek_token_is(parser, TOKEN_SEMICOLON)) {
		next_token(parser);
	}

	return node;
}

static ast_node_t *parse_let_statement(parser_t *parser) {
	ast_node_t *node = ast_node_stmt_new(AST_STMT_LET);
	if (node == NULL) {
		return NULL;
	}

	node->data.stmt->data.let->token = parser->cur_token;

	if (!expect_peek(parser, TOKEN_IDENT)) {
		// TODO: free
		return NULL;
	}

	node->data.stmt->data.let->name = malloc(sizeof(ast_expr_ident_t)); // TODO
	if (node->data.stmt->data.let->name == NULL) {
		// TODO: free
		return NULL;
	}

	node->data.stmt->data.let->name->token = parser->cur_token;
	node->data.stmt->data.let->name->value = parser->cur_token->literal;

	if (!expect_peek(parser, TOKEN_ASSIGN)) {
		// TODO: free
		return NULL;
	}

	next_token(parser);

	node->data.stmt->data.let->value = parse_expression(parser, PRECEDENCE_LOWEST);

	if (peek_token_is(parser, TOKEN_SEMICOLON)) {
		next_token(parser);
	}

	return node;
}

static ast_node_t *parse_return_statement(parser_t *parser) {
	ast_node_t *node = ast_node_stmt_new(AST_STMT_RETURN);
	if (node == NULL) {
		return NULL;
	}

	node->data.stmt->data.ret->token = parser->cur_token;

	next_token(parser);

	node->data.stmt->data.ret->return_value = parse_expression(parser, PRECEDENCE_LOWEST);

	if (peek_token_is(parser, TOKEN_SEMICOLON)) {
		next_token(parser);
	}

	return node;
}

static ast_node_t *parse_expression(parser_t *parser, op_precedence_t precedence) {
	prefix_parse_fn prefix = parser->prefix_parse_fns[parser->cur_token->type];
	if (prefix == NULL) {
		return NULL;
	}

	ast_node_t *left_expr = prefix(parser);

	while (!peek_token_is(parser, TOKEN_SEMICOLON) && precedence < peek_precedence(parser)) {
		infix_parse_fn infix = parser->infix_parse_fns[parser->peek_token->type];
		if (infix == NULL) {
			return left_expr;
		}

		next_token(parser);

		left_expr = infix(parser, left_expr);
	}

	return left_expr;
}

static ast_node_t *parse_boolean(parser_t *parser) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_BOOL);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.boolean->token = parser->cur_token;
	node->data.expr->data.boolean->value = cur_token_is(parser, TOKEN_TRUE);

	return node;
}

static ast_node_t *parse_call_expression(parser_t *parser, ast_node_t *function) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_CALL);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.call->token = parser->cur_token;
	node->data.expr->data.call->function = function;
	node->data.expr->data.call->arguments = parse_call_arguments(parser);

	return node;
}

static ast_node_t *parse_function_literal(parser_t *parser) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_FN_LIT);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.fn_lit->token = parser->cur_token;

	if (!expect_peek(parser, TOKEN_LPAREN)) {
		// TODO: free
		return NULL;
	}

	node->data.expr->data.fn_lit->parameters = parse_function_parameters(parser);

	if (!expect_peek(parser, TOKEN_LBRACE)) {
		// TODO: free
		return NULL;
	}

	node->data.expr->data.fn_lit->body = parse_block_statement(parser);

	return node;
}

static ast_node_t *parse_grouped_expression(parser_t *parser) {
	next_token(parser);

	ast_node_t *node = parse_expression(parser, PRECEDENCE_LOWEST);

	if (!peek_token_is(parser, TOKEN_RPAREN)) {
		return NULL;
	}

	return node;
}

static ast_node_t *parse_identifier(parser_t *parser) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_IDENT);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.ident->token = parser->cur_token;
	node->data.expr->data.ident->value = parser->cur_token->literal;

	return node;
}

static ast_node_t *parse_if_expression(parser_t *parser) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_IF);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.if_expr->token = parser->cur_token;

	if (!expect_peek(parser, TOKEN_LPAREN)) {
		// TODO: free
		return NULL;
	}

	next_token(parser);
	node->data.expr->data.if_expr->condition = parse_expression(parser, PRECEDENCE_LOWEST);

	if (!expect_peek(parser, TOKEN_RPAREN)) {
		// TODO: free
		return NULL;
	}

	if (!expect_peek(parser, TOKEN_LBRACE)) {
		// TODO: free
		return NULL;
	}

	node->data.expr->data.if_expr->consequence = parse_block_statement(parser);

	if (peek_token_is(parser, TOKEN_ELSE)) {
		next_token(parser);

		if (!expect_peek(parser, TOKEN_LBRACE)) {
			// TODO: free
			return NULL;
		}

		node->data.expr->data.if_expr->alternative = parse_block_statement(parser);
	}

	return node;
}

static ast_node_t *parse_infix_expression(parser_t *parser, ast_node_t *left) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_INFIX);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.infix->token = parser->cur_token;
	node->data.expr->data.infix->op = parser->cur_token->literal;
	node->data.expr->data.infix->left = left;

	op_precedence_t precedence = cur_precedence(parser);
	next_token(parser);
	node->data.expr->data.infix->right = parse_expression(parser, precedence);

	return node;
}

static ast_node_t *parse_integer_literal(parser_t *parser) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_INT_LIT);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.int_lit->token = parser->cur_token;
	node->data.expr->data.int_lit->value = strtoll(parser->cur_token->literal, NULL, 10);

	return node;
}

static ast_node_t *parse_prefix_expression(parser_t *parser) {
	ast_node_t *node = ast_node_expr_new(AST_EXPR_PREFIX);
	if (node == NULL) {
		exit(1);
	}

	node->data.expr->data.prefix->token = parser->cur_token;
	node->data.expr->data.prefix->op = parser->cur_token->literal;

	next_token(parser);

	node->data.expr->data.prefix->right = parse_expression(parser, PRECEDENCE_PREFIX);

	return node;
}

static list_t *parse_call_arguments(parser_t *parser) {
	list_t *args = list_new(1);
	if (args == NULL) {
		return NULL;
	}

	if (peek_token_is(parser, TOKEN_RPAREN)) {
		next_token(parser);
		return args;
	}

	next_token(parser);

	do {
		list_append(args, parse_expression(parser, PRECEDENCE_LOWEST));
		next_token(parser);
		next_token(parser);
	} while (peek_token_is(parser, TOKEN_COMMA));

	if (!expect_peek(parser, TOKEN_RPAREN)) {
		list_free(args);
		return NULL;
	}

	return args;
}

static list_t *parse_function_parameters(parser_t *parser) {
	list_t *identifiers = list_new(1);
	if (identifiers == NULL) {
		exit(1);
	}

	if (peek_token_is(parser, TOKEN_RPAREN)) {
		next_token(parser);
		return identifiers;
	}

	next_token(parser);

	do {
		ast_expr_ident_t *ident = malloc(sizeof(ast_expr_ident_t));
		if (ident == NULL) {
			exit(1);
		}

		ident->token = parser->cur_token;
		ident->value = parser->cur_token->literal;

		list_append(identifiers, ident);

		next_token(parser);
		next_token(parser);
	} while (peek_token_is(parser, TOKEN_COMMA));

	if (!expect_peek(parser, TOKEN_RPAREN)) {
		list_free(identifiers);
		return NULL;
	}

	return identifiers;
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

static void register_prefix(parser_t *parser, token_type_t type, prefix_parse_fn fn) {
	parser->prefix_parse_fns[type] = fn;
}

static void register_infix(parser_t *parser, token_type_t type, infix_parse_fn fn) {
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
