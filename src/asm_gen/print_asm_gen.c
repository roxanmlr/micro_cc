#include "asm_gen.h"

void print_asm_gen(asm_gen_t *asm_gen, FILE *out){
	if (!asm_gen || !out)
		return ;
	for (size_t i = 0; i < asm_gen->len; i++)
		fprintf(out, "%s\n", asm_gen->lines[i]);
}