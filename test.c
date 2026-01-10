#include "test.h"

#include "lexer.h"
#include "token.h"

#include <string.h>

void test_lexer(void) {
	char *input = "let five = 5;\nlet ten = 10;\n\nlet add = fn(x, y) {\n\tx + y;\n};\n\nlet result = add(five, ten);\n";

	struct test {
		token_type_t expected_type;
		char *expected_literal;
	};

	struct test tests[] = {
		{.expected_type=TOKEN_LET, .expected_literal="let"},
		{.expected_type=TOKEN_IDENT, .expected_literal="five"},
		{.expected_type=TOKEN_ASSIGN, .expected_literal="="},
		{.expected_type=TOKEN_INT, .expected_literal="5"},
		{.expected_type=TOKEN_SEMICOLON, .expected_literal=";"},
		{.expected_type=TOKEN_LET, .expected_literal="let"},
		{.expected_type=TOKEN_IDENT, .expected_literal="ten"},
		{.expected_type=TOKEN_ASSIGN, .expected_literal="="},
		{.expected_type=TOKEN_INT, .expected_literal="10"},
		{.expected_type=TOKEN_SEMICOLON, .expected_literal=";"},
		{.expected_type=TOKEN_LET, .expected_literal="let"},
		{.expected_type=TOKEN_IDENT, .expected_literal="add"},
		{.expected_type=TOKEN_ASSIGN, .expected_literal="="},
		{.expected_type=TOKEN_FUNCTION, .expected_literal="fn"},
		{.expected_type=TOKEN_LPAREN, .expected_literal="("},
		{.expected_type=TOKEN_IDENT, .expected_literal="x"},
		{.expected_type=TOKEN_COMMA, .expected_literal=","},
		{.expected_type=TOKEN_IDENT, .expected_literal="y"},
		{.expected_type=TOKEN_RPAREN, .expected_literal=")"},
		{.expected_type=TOKEN_LBRACE, .expected_literal="{"},
		{.expected_type=TOKEN_IDENT, .expected_literal="x"},
		{.expected_type=TOKEN_PLUS, .expected_literal="+"},
		{.expected_type=TOKEN_IDENT, .expected_literal="y"},
		{.expected_type=TOKEN_SEMICOLON, .expected_literal=";"},
		{.expected_type=TOKEN_RBRACE, .expected_literal="}"},
		{.expected_type=TOKEN_SEMICOLON, .expected_literal=";"},
		{.expected_type=TOKEN_LET, .expected_literal="let"},
		{.expected_type=TOKEN_IDENT, .expected_literal="result"},
		{.expected_type=TOKEN_ASSIGN, .expected_literal="="},
		{.expected_type=TOKEN_IDENT, .expected_literal="add"},
		{.expected_type=TOKEN_LPAREN, .expected_literal="("},
		{.expected_type=TOKEN_IDENT, .expected_literal="five"},
		{.expected_type=TOKEN_COMMA, .expected_literal=","},
		{.expected_type=TOKEN_IDENT, .expected_literal="ten"},
		{.expected_type=TOKEN_RPAREN, .expected_literal=")"},
		{.expected_type=TOKEN_SEMICOLON, .expected_literal=";"},
		{.expected_type=TOKEN_EOF, .expected_literal=""}
	};

	lexer_t *l = new_lexer(input);

	for (int i = 0; i < 37; ++i) {
		token_t *token = next_token(l);

		ASSERT_TRUE(token->type == tests[i].expected_type, "wrong token");
		ASSERT_TRUE(strcmp(token->literal, tests[i].expected_literal) == 0, "wrong literal");
	}
}

int main(void) {
	test_lexer();

	return 0;
}
