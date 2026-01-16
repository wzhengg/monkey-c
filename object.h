#pragma once

typedef enum obj_type obj_type_t;
typedef struct obj obj_t;
typedef union obj_data obj_data_t;
typedef struct obj_boolean obj_boolean_t;
typedef struct obj_integer obj_integer_t;
typedef struct obj_null obj_null_t;

typedef enum obj_type {
	OBJ_BOOLEAN,
	OBJ_INTEGER,
	OBJ_NULL,
} obj_type_t;

typedef struct obj {
	obj_type_t type;
	obj_data_t data;
} obj_t;

typedef union obj_data {
	obj_boolean_t *boolean;
	obj_integer_t *integer;
	obj_null_t *null;
} obj_data_t;

typedef struct obj_boolean {
	bool value;
} obj_boolean_t;

typedef struct obj_integer {
	long long value;
} obj_integer_t;

typedef struct obj_null {
} obj_null_t;

obj_t *obj_boolean_new(bool value);
obj_t *obj_integer_new(long long value);
