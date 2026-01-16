#include "ast.h"

#include "list.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static ast_node_t *ast_node_new(ast_node_type_t type);
static ast_expr_t *ast_expr_new(ast_expr_type_t type);
static ast_stmt_t *ast_stmt_new(ast_stmt_type_t type);

ast_node_t *ast_node_expr_new(ast_expr_type_t type) {
	ast_node_t *node = ast_node_new(AST_NODE_EXPR);
	if (node == NULL) {
		return NULL;
	}

	node->data.expr = ast_expr_new(type);
	if (node->data->expr == NULL) {
		free(node);
		return NULL;
	}

	return node;
}

ast_node_t *ast_node_prog_new(size_t capacity) {
	ast_node_t *node = ast_node_new(AST_NODE_PROG);
	if (node == NULL) {
		return NULL;
	}

	node->data.prog = malloc(sizeof(ast_prog_t));
	if (node->data.prog == NULL) {
		free(node);
		return NULL;
	}

	node->data.prog->stmts = list_new(capacity);
	if (node->data.prog->stmts == NULL) {
		free(node->data.prog);
		free(node);
		return NULL;
	}

	return node;
}

ast_node_t *ast_node_stmt_new(ast_stmt_type_t type) {
	ast_node_t *node = ast_node_new(AST_NODE_STMT);
	if (node == NULL) {
		return NULL;
	}

	node->data.stmt = ast_stmt_new(type);
	if (node->data.stmt == NULL) {
		free(node);
		return NULL;
	}

	return node;
}

static ast_node_t *ast_node_new(ast_node_type_t type) {
	ast_node_t *node = malloc(sizeof(ast_node_t));
	if (node == NULL) {
		return NULL;
	}

	node->type = type;

	return node;
}

static ast_expr_t *ast_expr_new(ast_expr_type_t type) {
	ast_expr_t *expr = malloc(sizeof(ast_expr_t));
	if (expr == NULL) {
		return NULL;
	}

	expr->type = type;

	void *ptr;
	switch (expr->type) {
		case AST_EXPR_BOOL:
			ptr = malloc(sizeof(ast_expr_bool_t));
			expr->data.boolean = ptr;
			break;
		case AST_EXPR_CALL:
			ptr = malloc(sizeof(ast_expr_call_t));
			expr->data.call = ptr;
			break;
		case AST_EXPR_FN_LIT:
			ptr = malloc(sizeof(ast_expr_fn_lit_t));
			expr->data.fn_lit = ptr;
			break;
		case AST_EXPR_IDENT:
			ptr = malloc(sizeof(ast_expr_ident_t));
			expr->data.ident = ptr;
			break;
		case AST_EXPR_IF:
			ptr = malloc(sizeof(ast_expr_if_t));
			expr->data.if_expr = ptr;
			break;
		case AST_EXPR_INFIX:
			ptr = malloc(sizeof(ast_expr_infix_t));
			expr->data.infix = ptr;
			break;
		case AST_EXPR_INT_LIT:
			ptr = malloc(sizeof(ast_expr_int_lit_t));
			expr->data.int_lit = ptr;
			break;
		case AST_EXPR_PREFIX:
			ptr = malloc(sizeof(ast_expr_prefix_t));
			expr->data.prefix = ptr;
			break;
		default:
			assert(false);
	}

	if (ptr == NULL) {
		free(expr);
		return NULL;
	}

	return expr;
}

static ast_stmt_t *ast_stmt_new(ast_stmt_type_t type) {
	ast_stmt_t *stmt = malloc(sizeof(ast_stmt_t));
	if (stmt == NULL) {
		return NULL;
	}

	stmt->type = type;

	void *ptr;
	switch (stmt->type) {
		case AST_STMT_BLOCK:
			ptr = malloc(sizeof(ast_stmt_block_t))
			stmt->data.block = ptr;
			break;
		case AST_STMT_EXPR:
			ptr = malloc(sizeof(ast_stmt_expr_t))
			stmt->data.expr = ptr;
			break;
		case AST_STMT_LET:
			ptr = malloc(sizeof(ast_stmt_let_t));
			stmt->data.let = ptr;
			break;
		case AST_STMT_RETURN:
			ptr = malloc(sizeof(ast_stmt_return_t));
			stmt->data.ret = ptr;
			break;
		default:
			assert(false);
	}

	if (ptr == NULL) {
		free(stmt);
		return NULL;
	}

	return stmt;
}

void ast_prog_stmts_append(ast_prog_t *prog, ast_node_t *stmt) {
	assert(prog != NULL);
	assert(prog->stmts != NULL);
	assert(stmt != NULL);
	list_append(prog->stmts, stmt);
}

ast_node_t *ast_prog_stmts_get(ast_prog_t *prog, int index) {
	assert(prog != NULL);
	assert(prog->stmts != NULL);

	if (index < 0 || index >= prog->stmts->size) {
		return NULL;
	}

	return (ast_stmt_t *) prog->stmts->arr[index];
}
