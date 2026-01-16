#include "evaluator.h"

#include "list.h"

#include <stddef.h>

static obj_t *eval_expression(ast_expr_t *expr);
static obj_t *eval_statement(ast_stmt_t *stmt);
static obj_t *eval_statements(list_t *stmts);

obj_t *eval(ast_node_t *node) {
	switch (node->type) {
		case AST_NODE_PROG:
			return eval_statements(node->data.prog->stmts);
		case AST_NODE_STMT:
	}

	return NULL;
}

static obj_t *eval_expression(ast_expr_t *expr) {
	switch (expr->type) {
		case AST_EXPR_BOOL:
			return obj_boolean_new(expr->data.boolean->value);
		case AST_EXPR_INT_LIT:
			return obj_integer_new(expr->data.int_lit->value);
	}

	return NULL;
}

static obj_t *eval_statement(ast_stmt_t *stmt) {
	switch (stmt->type) {
		case AST_STMT_EXPR:
			return eval(stmt->data.expr);
	}

	return NULL;
}

static obj_t *eval_statements(list_t *stmts) {
	obj_t *result = NULL;;

	for (int i = 0; i < stmts->size; ++i) {
		result = eval(stmts->arr[i]);
	}

	return result;
}
