#include "token.h"

#include <string.h>

token_type_t lookup_ident(char *ident) {
	if (strcmp(ident, "let") == 0) {
		return TOKEN_LET;
	}
	if (strcmp(ident, "fn") == 0) {
		return TOKEN_FUNCTION;
	}
	return TOKEN_IDENT;
}
