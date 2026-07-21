#ifndef ASM_GEN_H
#define ASM_GEN_H
#include "../parser/parser.h"
#include <stdio.h>

typedef struct asm_gen {
	size_t cap;
	size_t len;
	char **lines;
} asm_gen_t;

asm_gen_t *asm_gen(struct program_node *ast);
void free_asm_gen(asm_gen_t *asm_gen);
void print_asm_gen(asm_gen_t *asm_gen, FILE *out);
#endif