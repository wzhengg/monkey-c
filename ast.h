#pragma once

#include "list.h"
#include "token.h"

typedef enum ast_node_type {
	AST_NODE_EXPR,
	AST_NODE_PROG,
	AST_NODE_STMT,
} ast_node_type_t;

typedef enum ast_expr_type {
	AST_EXPR_BOOL,
	AST_EXPR_CALL,
	AST_EXPR_FN_LIT,
	AST_EXPR_IDENT,
	AST_EXPR_IF,
	AST_EXPR_INFIX,
	AST_EXPR_INT_LIT,
	AST_EXPR_PREFIX,
} ast_expr_type_t;

typedef enum ast_stmt_type {
	AST_STMT_BLOCK,
	AST_STMT_EXPR,
	AST_STMT_LET,
	AST_STMT_RETURN,
} ast_stmt_type_t;

typedef struct ast_node ast_node_t;

typedef struct ast_expr ast_expr_t;
typedef struct ast_prog ast_prog_t;
typedef struct ast_stmt ast_stmt_t;

typedef struct ast_expr_bool ast_expr_bool_t;
typedef struct ast_expr_call ast_expr_call_t;
typedef struct ast_expr_fn_lit ast_expr_fn_lit_t;
typedef struct ast_expr_ident ast_expr_ident_t;
typedef struct ast_expr_if ast_expr_if_t;
typedef struct ast_expr_infix ast_expr_infix_t;
typedef struct ast_expr_int_lit ast_expr_int_lit_t;
typedef struct ast_expr_prefix ast_expr_prefix_t;

typedef struct ast_stmt_block ast_stmt_block_t;
typedef struct ast_stmt_expr ast_stmt_expr_t;
typedef struct ast_stmt_let ast_stmt_let_t;
typedef struct ast_stmt_return ast_stmt_return_t;

typedef union ast_node_data ast_node_data_t;
typedef union ast_expr_data ast_expr_data_t;
typedef union ast_stmt_data ast_stmt_data_t;

typedef union ast_node_data {
	ast_expr_t *expr;
	ast_prog_t *prog;
	ast_stmt_t *stmt;
} ast_node_data_t;

typedef union ast_expr_data {
	ast_expr_bool_t *boolean;
	ast_expr_call_t *call;
	ast_expr_fn_lit_t *fn_lit;
	ast_expr_ident_t *ident;
	ast_expr_if_t *if_expr;
	ast_expr_infix_t *infix;
	ast_expr_int_lit_t *int_lit;
	ast_expr_prefix_t *prefix;
} ast_expr_data_t;

typedef union ast_stmt_data {
	ast_stmt_block_t *block;
	ast_stmt_expr_t *expr;
	ast_stmt_let_t *let;
	ast_stmt_return_t *ret;
} ast_stmt_data_t;

typedef struct ast_node {
	ast_node_type_t type;
	ast_node_data_t data;
} ast_node_t;

typedef struct ast_expr {
	ast_expr_type_t type;
	ast_expr_data_t data;
} ast_expr_t;

typedef struct ast_prog {
	list_t *stmts;
} ast_prog_t;

typedef struct ast_stmt {
	ast_stmt_type_t type;
	ast_stmt_data_t data;
} ast_stmt_t;

typedef struct ast_expr_bool {
	token_t *token;
	bool value;
} ast_expr_bool_t;

typedef struct ast_expr_call {
	token_t *token;
	ast_node_t *function;
	list_t *arguments;
} ast_expr_call_t;

typedef struct ast_expr_fn_lit {
	token_t *token;
	list_t *parameters;
	ast_node_t *body;
} ast_expr_fn_lit_t;

typedef struct ast_expr_ident {
	token_t *token;
	char *value;
} ast_expr_ident_t;

typedef struct ast_expr_if {
	token_t *token;
	ast_node_t *condition;
	ast_node_t *consequence;
	ast_node_t *alternative;
} ast_expr_if_t;

typedef struct ast_expr_infix {
	token_t *token;
	ast_node_t *left;
	char *op;
	ast_node_t *right;
} ast_expr_infix_t;

typedef struct ast_expr_int_lit {
	token_t *token;
	long long value;
} ast_expr_int_lit_t;

typedef struct ast_expr_prefix {
	token_t *token;
	char *op;
	ast_node_t *right;
} ast_expr_prefix_t;

typedef struct ast_stmt_block {
	token_t *token;
	list_t *stmts;
} ast_stmt_block_t;

typedef struct ast_stmt_expr {
	token_t *token;
	ast_node_t *expr;
} ast_stmt_expr_t;

typedef struct ast_stmt_let {
	token_t *token;
	ast_node_t *name; // TODO: change to ast_node_t*
	ast_node_t *value;
} ast_stmt_let_t;

typedef struct ast_stmt_return {
	token_t *token;
	ast_node_t *return_value;
} ast_stmt_return_t;

ast_node_t *ast_node_expr_new(ast_expr_type_t type);
ast_node_t *ast_node_prog_new(size_t capacity);
ast_node_t *ast_node_stmt_new(ast_stmt_type_t type);

void ast_prog_stmts_append(ast_prog_t *prog, ast_node_t *stmt);
ast_node_t *ast_prog_stmts_get(ast_prog_t *prog, int index);
