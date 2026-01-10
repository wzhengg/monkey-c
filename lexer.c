#include "lexer.h"

#include "token.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void read_char(lexer_t *l);
static token_t *new_token(token_type_t type, char *literal);
static char *read_identifier(lexer_t *l);
static char *read_number(lexer_t *l);
static bool is_letter(char ch);
static bool is_digit(char ch);
static void skip_whitespace(lexer_t *l);

lexer_t *new_lexer(char *input) {
	lexer_t *l = malloc(sizeof *l);
	if (l == NULL) {
		return NULL;
	}

	size_t n = strlen(input);
	char *s = malloc(n + 1);
	if (s == NULL) {
		free(l);
		return NULL;
	}
	strcpy(s, input);

	l->input = s;
	l->n = n;
	l->position = 0;
	l->read_position = 0;
	l->ch = 0;

	read_char(l);

	return l;
}

static void read_char(lexer_t *l) {
	if (l->read_position >= l->n) {
		l->ch = 0;
	} else {
		l->ch = l->input[l->read_position];
	}
	l->position = l->read_position;
	++l->read_position;
}

token_t *next_token(lexer_t *l) {
	token_t *token = NULL;

	skip_whitespace(l);

	switch (l->ch) {
		case '=':
			token = new_token(TOKEN_ASSIGN, "=");
			break;
		case ';':
			token = new_token(TOKEN_SEMICOLON, ";");
			break;
		case '(':
			token = new_token(TOKEN_LPAREN, "(");
			break;
		case ')':
			token = new_token(TOKEN_RPAREN, ")");
			break;
		case ',':
			token = new_token(TOKEN_COMMA, ",");
			break;
		case '+':
			token = new_token(TOKEN_PLUS, "+");
			break;
		case '{':
			token = new_token(TOKEN_LBRACE, "{");
			break;
		case '}':
			token = new_token(TOKEN_RBRACE, "}");
			break;
		case '\0':
			token = new_token(TOKEN_EOF, "");
			break;
		default:
			if (is_letter(l->ch)) {
				char *ident = read_identifier(l);
				token_type_t type = lookup_ident(ident);
				token = new_token(type, ident);
				return token;
			} else if (is_digit(l->ch)) {
				token = new_token(TOKEN_INT, read_number(l));
				return token;
			} else {
				token = new_token(TOKEN_ILLEGAL, (char[2]){l->ch, '\0'});
			}
			break;
	}

	read_char(l);

	return token;
}

static token_t *new_token(token_type_t type, char *literal) {
	token_t *token = malloc(sizeof *token);
	if (token == NULL) {
		return NULL;
	}

	size_t n = strlen(literal);

	char *s = calloc(n + 1, sizeof *s);
	if (s == NULL) {
		free(token);
		return NULL;
	}

	strcpy(s, literal);

	token->type = type;
	token->literal = s;

	return token;
}

static char *read_identifier(lexer_t *l) {
	int position = l->position;

	while (is_letter(l->ch)) {
		read_char(l);
	}

	size_t n = l->position - position;

	char *identifier = calloc(n + 1, sizeof *identifier);
	if (identifier == NULL) {
		return NULL;
	}

	strncpy(identifier, &l->input[position], n);
	identifier[n] = '\0';

	return identifier;
}

static char *read_number(lexer_t *l) {
	int position = l->position;

	while (is_digit(l->ch)) {
		read_char(l);
	}

	size_t n = l->position - position;

	char *number = calloc(n + 1, sizeof *number);
	if (number == NULL) {
		return NULL;
	}

	strncpy(number, &l->input[position], n);
	number[n] = '\0';

	return number;
}

static bool is_letter(char ch) {
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

static bool is_digit(char ch) {
	return '0' <= ch && ch <= '9';
}

static void skip_whitespace(lexer_t *l) {
	while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
		read_char(l);
	}
}
