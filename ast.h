#pragma once

#include "list.h"
#include "token.h"

typedef struct ast_node ast_node_t;

typedef enum {
	AST_EXPRESSION,
	AST_PROGRAM,
	AST_STATEMENT,
} ast_node_type_t;

typedef enum {
	AST_BOOLEAN,
	AST_IDENTIFIER,
	AST_INT_LITERAL,
	AST_PREFIX,
	AST_INFIX,
} ast_expr_type_t;

typedef enum {
	AST_EXPR_STMT,
	AST_LET,
	AST_RETURN,
} ast_stmt_type_t;



typedef struct ast_expr ast_expr_t;

typedef struct {
	token_t *token;
	bool value;
} ast_bool_expr_t;

typedef struct {
	token_t *token;
	char *value;
} ast_ident_expr_t;

typedef struct {
	token_t *token;
	long long value;
} ast_int_lit_expr_t;

typedef struct {
	token_t *token;
	char *op;
	ast_expr_t *right;
} ast_prefix_expr_t;

typedef struct {
	token_t *token;
	ast_expr_t *left;
	char *op;
	ast_expr_t *right;
} ast_infix_expr_t;

typedef union {
	ast_bool_expr_t *boolean;
	ast_ident_expr_t *ident;
	ast_int_lit_expr_t *int_lit;
	ast_prefix_expr_t *prefix;
	ast_infix_expr_t *infix;
} ast_expr_data_t;

typedef struct ast_expr {
	ast_expr_type_t type;
	ast_expr_data_t data;
} ast_expr_t;



typedef struct {
	list_t *stmts;
} ast_prog_t;



typedef struct ast_expr_stmt {
	token_t *token;
	ast_expr_t *expr;
} ast_expr_stmt_t;

typedef struct ast_let_stmt {
	token_t *token;
	ast_ident_expr_t *name;
	ast_node_t *value;
} ast_let_stmt_t;

typedef struct ast_return_stmt {
	token_t *token;
	ast_node_t *return_value;
} ast_return_stmt_t;

typedef union {
	ast_expr_stmt_t *expr;
	ast_let_stmt_t *let;
	ast_return_stmt_t *ret;
} ast_stmt_data_t;

typedef struct {
	ast_stmt_type_t type;
	ast_stmt_data_t data;
} ast_stmt_t;



typedef union {
	ast_expr_t *expr;
	ast_prog_t *prog;
	ast_stmt_t *stmt;
} ast_node_data_t;

typedef struct ast_node {
	ast_node_type_t type;
	ast_node_data_t data;
} ast_node_t;



void ast_prog_stmts_append(ast_prog_t *prog, ast_stmt_t *stmt);
ast_stmt_t *ast_prog_stmts_get(ast_prog_t *prog, int index);
