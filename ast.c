#include "ast.h"

#include "list.h"

#include <stddef.h>
#include <assert.h>

void ast_prog_stmts_append(ast_prog_t *prog, ast_stmt_t *stmt) {
	assert(prog != NULL);
	assert(prog->stmts != NULL);
	assert(stmt != NULL);

	list_append(prog->stmts, (void *) stmt);
}

ast_stmt_t *ast_prog_stmts_get(ast_prog_t *prog, int index) {
	assert(prog != NULL);
	assert(prog->stmts != NULL);

	if (index < 0 || index >= prog->stmts->size) {
		return NULL;
	}

	return (ast_stmt_t *) prog->stmts->arr[index];
}
