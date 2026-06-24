#include "array.h"

array_t * arr_init(size_t cap){
	if (!cap)
		return NULL;
	array_t* arr = malloc(sizeof(array_t) + sizeof(void *)*cap);
	if (!arr)
		return NULL;
	memset(arr, 0, sizeof(array_t) + sizeof(void *)*cap);
	arr->cap = cap;
	return arr;
}

void arr_free(array_t* arr, void (*free_elem)(void *)){
	if (!arr)
		return;
	for (u_int32_t i = 0; i < arr->len; i++)
		free_elem((arr->data)[i]);
	free(arr);
}

ssize_t arr_grow(array_t **arr, size_t addcap){
	if (!arr || !addcap)
		return -1;
	size_t oldcap = *arr ? (*arr)->cap:0;
	size_t newcap = oldcap + addcap;
	array_t *newarr = arr_init(newcap);
	if (!newarr)
		return -1;
	if (*arr)
		memmove(newarr,*arr,sizeof(array_t) + sizeof(void*)*oldcap);
	newarr->cap = newcap;
	free(*arr);
	*arr = newarr;
	return newarr->cap;
}

int arr_append(array_t *arr, void *value){
	if(!arr || arr->len >= arr->cap)
		return 0;
	(arr->data)[arr->len] = value;
	(arr->len)++;
	return 1;
}