#ifndef UTILS_H
#define UTILS_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void put_n_char(char c, int n, int fd);
ssize_t str_grow(char **p_str, char *append);
#endif