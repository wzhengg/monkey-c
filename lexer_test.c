#include "lexer_test.h"

#include "lexer.h"
#include "token.h"

#include <assert.h>
#include <string.h>


void test_lexer(void) {
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
