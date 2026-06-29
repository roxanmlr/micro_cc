#ifndef ARRAY_H
#define ARRAY_H
#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct array {
	size_t len;
	size_t cap;
	void *data[];
};

typedef struct array array_t;

array_t *arr_init(size_t cap);
void arr_free(array_t * arr, void (*free_elem)(void *));
ssize_t arr_grow(array_t ** arr, size_t addcap);
int arr_append(array_t * arr, void *value);
#endif
