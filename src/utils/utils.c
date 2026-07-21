#include "utils.h"

void put_n_char(char c, int n, int fd){
	char buf[n];
	memset(buf, c, n);
	write(fd, buf, n);
}

ssize_t str_grow(char **p_str, char *append){
	if (!p_str)
		return -1;
	if (!*p_str){
		*p_str = strdup(append);
		return (*p_str != NULL) ? strlen(*p_str):-1;
	}
	char *new_str = calloc(strlen(*p_str) + strlen(append) + 1, sizeof(char));
	if (!new_str)
		return -1;
	memmove(new_str, *p_str, strlen(*p_str));
	memmove(new_str + strlen(*p_str), append, strlen(append));
	free(*p_str);
	*p_str = new_str;
	return strlen(*p_str);
}