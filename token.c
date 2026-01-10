#include "token.h"

#include <string.h>

token_type_t lookup_ident(char *ident) {
	if (strcmp(ident, "let") == 0) {
		return TOKEN_LET;
	}
	if (strcmp(ident, "fn") == 0) {
		return TOKEN_FUNCTION;
	}
	if (strcmp(ident, "true") == 0) {
		return TOKEN_TRUE;
	}
	if (strcmp(ident, "false") == 0) {
		return TOKEN_FALSE;
	}
	if (strcmp(ident, "if") == 0) {
		return TOKEN_IF;
	}
	if (strcmp(ident, "else") == 0) {
		return TOKEN_ELSE;
	}
	if (strcmp(ident, "return") == 0) {
		return TOKEN_RETURN;
	}

	return TOKEN_IDENT;
}
