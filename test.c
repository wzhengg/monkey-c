#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

#include <assert.h>
#include <string.h>

static void test_lexer() {
	char *input =
		"let five = 5;\n"
		"let ten = 10;\n"
		"\n"
		"let add = fn(x, y) {\n"
		"\tx + y;\n"
		"};\n"
		"\n"
		"let result = add(five, ten);\n"
		"!-/*5;\n"
		"5 < 10 > 5;\n"
		"\n"
		"if (5 < 10) {\n"
		"\treturn true;\n"
		"} else {\n"
		"\treturn false;\n"
		"}\n"
		"\n"
		"10 == 10;\n"
		"10 != 9;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	token_t *token;


	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LET);
	assert(strcmp(token->literal, "let") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "five") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_ASSIGN);
	assert(strcmp(token->literal, "=") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "5") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LET);
	assert(strcmp(token->literal, "let") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "ten") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_ASSIGN);
	assert(strcmp(token->literal, "=") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "10") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LET);
	assert(strcmp(token->literal, "let") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "add") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_ASSIGN);
	assert(strcmp(token->literal, "=") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_FUNCTION);
	assert(strcmp(token->literal, "fn") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LPAREN);
	assert(strcmp(token->literal, "(") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "x") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_COMMA);
	assert(strcmp(token->literal, ",") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "y") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RPAREN);
	assert(strcmp(token->literal, ")") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LBRACE);
	assert(strcmp(token->literal, "{") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "x") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_PLUS);
	assert(strcmp(token->literal, "+") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "y") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RBRACE);
	assert(strcmp(token->literal, "}") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LET);
	assert(strcmp(token->literal, "let") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "result") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_ASSIGN);
	assert(strcmp(token->literal, "=") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "add") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LPAREN);
	assert(strcmp(token->literal, "(") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "five") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_COMMA);
	assert(strcmp(token->literal, ",") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IDENT);
	assert(strcmp(token->literal, "ten") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RPAREN);
	assert(strcmp(token->literal, ")") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_BANG);
	assert(strcmp(token->literal, "!") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_MINUS);
	assert(strcmp(token->literal, "-") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SLASH);
	assert(strcmp(token->literal, "/") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_ASTERISK);
	assert(strcmp(token->literal, "*") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "5") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "5") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LT);
	assert(strcmp(token->literal, "<") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "10") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_GT);
	assert(strcmp(token->literal, ">") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "5") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_IF);
	assert(strcmp(token->literal, "if") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LPAREN);
	assert(strcmp(token->literal, "(") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "5") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LT);
	assert(strcmp(token->literal, "<") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "10") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RPAREN);
	assert(strcmp(token->literal, ")") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LBRACE);
	assert(strcmp(token->literal, "{") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RETURN);
	assert(strcmp(token->literal, "return") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_TRUE);
	assert(strcmp(token->literal, "true") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RBRACE);
	assert(strcmp(token->literal, "}") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_ELSE);
	assert(strcmp(token->literal, "else") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_LBRACE);
	assert(strcmp(token->literal, "{") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RETURN);
	assert(strcmp(token->literal, "return") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_FALSE);
	assert(strcmp(token->literal, "false") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_RBRACE);
	assert(strcmp(token->literal, "}") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "10") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_EQ);
	assert(strcmp(token->literal, "==") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "10") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "10") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_NEQ);
	assert(strcmp(token->literal, "!=") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_INT);
	assert(strcmp(token->literal, "9") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_SEMICOLON);
	assert(strcmp(token->literal, ";") == 0);

	token = lexer_next_token(lexer);
	assert(token != NULL);
	assert(token->type == TOKEN_EOF);
	assert(strcmp(token->literal, "") == 0);
}

static void test_parser_let_statements() {
	char *input =
		"let x = 5;\n"
		"let y = 10;\n"
		"let foobar = 838383;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = parser_parse_program(parser);
	assert(prog != NULL);
	assert(prog->type == AST_PROGRAM);
	assert(prog->data.prog->stmts->size == 3);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_LET);
	assert(strcmp(stmt->data.let->token->literal, "let") == 0);
	assert(strcmp(stmt->data.let->name->value, "x") == 0);
	assert(strcmp(stmt->data.let->name->token->literal, "x") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 1);
	assert(stmt != NULL);
	assert(stmt->type == AST_LET);
	assert(strcmp(stmt->data.let->token->literal, "let") == 0);
	assert(strcmp(stmt->data.let->name->value, "y") == 0);
	assert(strcmp(stmt->data.let->name->token->literal, "y") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 2);
	assert(stmt != NULL);
	assert(stmt->type == AST_LET);
	assert(strcmp(stmt->data.let->token->literal, "let") == 0);
	assert(strcmp(stmt->data.let->name->value, "foobar") == 0);
	assert(strcmp(stmt->data.let->name->token->literal, "foobar") == 0);
}

static void test_parser_return_statements() {
	char *input =
		"return 5;\n"
		"return 10;\n"
		"return 993322;";

	lexer_t *lexer = lexer_new(input);
	assert(lexer != NULL);
	parser_t *parser = parser_new(lexer);
	assert(parser != NULL);

	ast_node_t *prog = parser_parse_program(parser);
	assert(prog != NULL);
	assert(prog->type == AST_PROGRAM);
	assert(prog->data.prog->stmts->size == 3);

	ast_stmt_t *stmt = ast_prog_stmts_get(prog->data.prog, 0);
	assert(stmt != NULL);
	assert(stmt->type == AST_RETURN);
	assert(strcmp(stmt->data.ret->token->literal, "return") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 1);
	assert(stmt != NULL);
	assert(stmt->type == AST_RETURN);
	assert(strcmp(stmt->data.ret->token->literal, "return") == 0);

	stmt = ast_prog_stmts_get(prog->data.prog, 2);
	assert(stmt != NULL);
	assert(stmt->type == AST_RETURN);
	assert(strcmp(stmt->data.ret->token->literal, "return") == 0);
}

int main(void) {
	test_lexer();
	test_parser_let_statements();
	test_parser_return_statements();

	return 0;
}
