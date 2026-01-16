#include "parser_test.h"

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

#include <assert.h>
#include <string.h>

static void test_let_statements(void);
static void test_return_statements(void);
static void test_identifier_expression(void);
static void test_integer_literal_expression(void);
static void test_prefix_expression_bang(void);
static void test_prefix_expression_minus(void);
static void test_infix_expressions(void);
static void test_integer_literal(ast_expr_t *expr, long long value);
static void test_infix_expression(char *input, long long left, char *op, long long right);

void test_parser(void) {
	test_let_statements();
	test_return_statements();
	test_identifier_expression();
	test_integer_literal_expression();
	test_prefix_expression_bang();
	test_prefix_expression_minus();
	test_infix_expressions();
}

static void test_let_statements(void) {
	char *input =
		"let x = 5;\n"
		"let y = 10;\n"
		"let foobar = 838383;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = infix_parse_fn(parser);
	assert(prog != NULL);
	assert(prog->type == AST_NODE_PROG);
	assert(prog->data.prog->stmts->size == 3);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_LET);
	assert(strcmp(stmt->data.let->token->literal, "let") == 0);
	assert(strcmp(stmt->data.let->name->value, "x") == 0);
	assert(strcmp(stmt->data.let->name->token->literal, "x") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 1);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_LET);
	assert(strcmp(stmt->data.let->token->literal, "let") == 0);
	assert(strcmp(stmt->data.let->name->value, "y") == 0);
	assert(strcmp(stmt->data.let->name->token->literal, "y") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 2);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_LET);
	assert(strcmp(stmt->data.let->token->literal, "let") == 0);
	assert(strcmp(stmt->data.let->name->value, "foobar") == 0);
	assert(strcmp(stmt->data.let->name->token->literal, "foobar") == 0);
}

static void test_return_statements(void) {
	char *input =
		"return 5;\n"
		"return 10;\n"
		"return 993322;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = infix_parse_fn(parser);
	assert(prog != NULL);
	assert(prog->type == AST_NODE_PROG);
	assert(prog->data.prog->stmts->size == 3);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_RETURN);
	assert(strcmp(stmt->data.ret->token->literal, "return") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 1);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_RETURN);
	assert(strcmp(stmt->data.ret->token->literal, "return") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 2);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_RETURN);
	assert(strcmp(stmt->data.ret->token->literal, "return") == 0);
}

static void test_identifier_expression(void) {
	char *input = "foobar;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = infix_parse_fn(parser);
	assert(prog != NULL);
	assert(prog->data.prog->stmts != NULL);
	assert(prog->data.prog->stmts->size == 1);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_EXPR);

	ast_stmt_expr_t *expr_stmt = stmt->data.expr;
	assert(expr_stmt != NULL);

	ast_expr_t *expr = expr_stmt->expr;
	assert(expr != NULL);
	assert(expr->type == AST_EXPR_IDENT);

	ast_expr_ident_t *ident = expr->data.ident;
	assert(ident != NULL);
	assert(strcmp(ident->value, "foobar") == 0);
	assert(ident->token != NULL);
	assert(strcmp(ident->token->literal, "foobar") == 0);
}

static void test_integer_literal_expression(void) {
	char *input = "5;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = infix_parse_fn(parser);
	assert(prog != NULL);
	assert(prog->data.prog->stmts != NULL);
	assert(prog->data.prog->stmts->size == 1);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_EXPR);

	ast_stmt_expr_t *expr_stmt = stmt->data.expr;
	assert(expr_stmt != NULL);

	ast_expr_t *expr = expr_stmt->expr;
	assert(expr != NULL);
	assert(expr->type == AST_EXPR_INT_LIT);

	ast_expr_int_lit_t *lit = expr->data.int_lit;
	assert(lit != NULL);
	assert(lit->value == 5);
	assert(lit->token != NULL);
	assert(strcmp(lit->token->literal, "5") == 0);
}

static void test_prefix_expression_bang(void) {
	char *input = "!5;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = infix_parse_fn(parser);
	assert(prog != NULL);
	assert(prog->data.prog->stmts != NULL);
	assert(prog->data.prog->stmts->size == 1);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_EXPR);

	ast_stmt_expr_t *expr_stmt = stmt->data.expr;
	assert(expr_stmt != NULL);

	ast_expr_t *expr = expr_stmt->expr;
	assert(expr != NULL);
	assert(expr->type == AST_EXPR_PREFIX);

	ast_expr_prefix_t *prefix = expr->data.prefix;
	assert(prefix != NULL);
	assert(strcmp(prefix->op, "!") == 0);

	test_integer_literal(prefix->right, 5);
}

static void test_prefix_expression_minus(void) {
	char *input = "-15;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = infix_parse_fn(parser);
	assert(prog != NULL);
	assert(prog->data.prog->stmts != NULL);
	assert(prog->data.prog->stmts->size == 1);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_EXPR);

	ast_stmt_expr_t *expr_stmt = stmt->data.expr;
	assert(expr_stmt != NULL);

	ast_expr_t *expr = expr_stmt->expr;
	assert(expr != NULL);
	assert(expr->type == AST_EXPR_PREFIX);

	ast_expr_prefix_t *prefix = expr->data.prefix;
	assert(prefix != NULL);
	assert(strcmp(prefix->op, "-") == 0);

	test_integer_literal(prefix->right, 15);
}

static void test_infix_expressions(void) {
	test_infix_expression("5 + 5;", 5, "+", 5);
	test_infix_expression("5 - 5;", 5, "-", 5);
	test_infix_expression("5 * 5;", 5, "*", 5);
	test_infix_expression("5 / 5;", 5, "/", 5);
	test_infix_expression("5 > 5;", 5, ">", 5);
	test_infix_expression("5 < 5;", 5, "<", 5);
	test_infix_expression("5 == 5;", 5, "==", 5);
	test_infix_expression("5 != 5;", 5, "!=", 5);
}

static void test_integer_literal(ast_expr_t *expr, long long value) {
	assert(expr->type == AST_EXPR_INT_LIT);

	ast_expr_int_lit_t *lit = expr->data.int_lit;
	assert(lit != NULL);

	assert(lit->value == value);
}

static void test_infix_expression(char *input, long long left, char *op, long long right) {
	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = infix_parse_fn(parser);
	assert(prog != NULL);
	assert(prog->data.prog->stmts != NULL);
	assert(prog->data.prog->stmts->size == 1);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_STMT_EXPR);

	ast_stmt_expr_t *expr_stmt = stmt->data.expr;
	assert(expr_stmt != NULL);

	ast_expr_t *expr = expr_stmt->expr;
	assert(expr != NULL);
	assert(expr->type == AST_EXPR_INFIX);

	ast_expr_infix_t *infix = expr->data.infix;
	assert(infix != NULL);
	assert(strcmp(infix->op, op) == 0);

	test_integer_literal(infix->left, left);
	test_integer_literal(infix->right, right);
}
