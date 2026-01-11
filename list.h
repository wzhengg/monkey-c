#pragma once

#include <stddef.h>

typedef struct {
	size_t size;
	size_t capacity;
	void **arr;
} list_t;

list_t *list_new(size_t capacity);
void list_free(list_t *l);
void list_append(list_t *l, void *obj);
