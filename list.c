#include "list.h"

#include <stddef.h>
#include <stdlib.h>

list_t *list_new(size_t capacity) {
	list_t *l = malloc(sizeof *l);
	if (l == NULL) {
		return NULL;
	}

	void **arr = calloc(capacity, sizeof(void *));
	if (arr == NULL) {
		free(l);
		return NULL;
	}

	l->size = 0;
	l->capacity = capacity;
	l->arr = arr;

	return l;
}

void list_free(list_t *l) {
	if (l == NULL) {
		return;
	}

	if (l->arr != NULL) {
		free(l->arr);
	}

	free(l);
}

void list_append(list_t *l, void *obj) {
	if (l->size == l->capacity) {
		size_t new_capacity = l->capacity * 2;

		void **new_arr = realloc(l->arr, new_capacity * sizeof(void *));
		if (new_arr == NULL) {
			exit(EXIT_FAILURE);
		}

		l->arr = new_arr;
		l->capacity = new_capacity;
	}

	l->arr[l->size++] = obj;
}
