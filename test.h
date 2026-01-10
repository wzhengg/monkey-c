#pragma once

#include <stdio.h>

#define ASSERT_TRUE(condition, message) \
	do { \
		if (!(condition)) { \
			fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, message); \
		} \
	} while (0)
