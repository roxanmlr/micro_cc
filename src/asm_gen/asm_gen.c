#include "asm_gen.h"
#include "../utils/utils.h"
#include <stdio.h>

void free_asm_gen(asm_gen_t *asm_gen){
	if (!asm_gen)
		return ;
	for (size_t i = 0; i < asm_gen->len; i++)
		free(asm_gen->lines[i]);
	free(asm_gen->lines);
	free(asm_gen);
}

static ssize_t asm_add_line(asm_gen_t *asm_gen, char *str){
	if (!asm_gen || !str)
		return -1;
	if (asm_gen->len == asm_gen->cap){
		size_t new_cap = asm_gen->cap * 2;
		char **lines = (char **)calloc(new_cap + 1, sizeof(char *));
		if (!lines)
			return -1;
		memmove(lines, asm_gen->lines, asm_gen->len);
		free(asm_gen->lines);
		asm_gen->lines = lines;
		asm_gen->cap = new_cap;
	}
	asm_gen->lines[asm_gen->len] = strdup(str);
	if (!asm_gen->lines[asm_gen->len])
		return -1;
	/*fprintf(stderr, "%s\n", asm_gen->lines[asm_gen->len]);*/
	(asm_gen->len)++;
	return strlen(str);
}

static ssize_t asm_gen_exp(asm_gen_t * asm_gen, struct exp_node *exp_node){
	if (!asm_gen || !exp_node)
		return -1;
	char str[1024];
	snprintf(str, 1024, "\tmovl\t$%d, %%eax", exp_node->constant);
	if (-1 == asm_add_line(asm_gen, str))
		return -1;
	return 0;
}

static ssize_t asm_gen_return(asm_gen_t *asm_gen, struct ret_node *ret_node){
	if (!asm_gen || !ret_node)
		return -1;
	if (-1 == asm_gen_exp(asm_gen, ret_node->node))
		return -1;
	if (-1 == asm_add_line(asm_gen, "\tret"))
		return -1;
	return 0;
}

static ssize_t asm_gen_stmt(asm_gen_t *asm_gen, struct stmt_node *stmt_node){
	if (!asm_gen || !stmt_node)
		return -1;
	if (-1 == asm_gen_return(asm_gen, stmt_node->node))
		return -1;
	return 0;
	
}

static ssize_t asm_gen_func(asm_gen_t *asm_gen, struct func_node *func_node){
	if (!asm_gen || !func_node)
		return -1;
	char *str = strdup("\t.globl\t");
	if (!str)
		goto clean;
	if (-1 == str_grow(&str, func_node->name))
		goto clean;
	if (-1 == asm_add_line(asm_gen, str))
		goto clean;
	free(str);
	str = strdup("\t.type\t");
	if (-1 == str_grow(&str, func_node->name))
		goto clean;
	if (-1 == str_grow(&str, ", @function"))
		goto clean;
	if (-1 == asm_add_line(asm_gen, str))
		goto clean;
	free(str);
	str = strdup(func_node->name);
	if (!str) goto clean;
	if (-1 == str_grow(&str, ":")) goto clean;
	if (-1 == asm_add_line(asm_gen, str)) goto clean;
	free(str);
	if (-1 == asm_gen_stmt(asm_gen, func_node->node)) goto clean;
	char buf[BUFSIZ];
	snprintf(buf, BUFSIZ,"\t.size\t%s, .-%s", func_node->name, func_node->name);
	if (-1 == asm_add_line(asm_gen, buf)) goto clean;
	return 0;
clean:
	free(str);
	return -1;
}

asm_gen_t *asm_gen(struct program_node *ast){
	if (!ast || !ast->len)
		return NULL;
	asm_gen_t * asm_gen = calloc(1, sizeof(asm_gen_t));
	if (!asm_gen){
		perror("calloc");
		return NULL;
	}
	asm_gen->cap = 64;
	asm_gen->lines = (char **)calloc(asm_gen->cap + 1, sizeof(char*));
	if (!asm_gen->lines)
		return free(asm_gen), NULL;
	/*TODO : Put real file name*/
	if (-1 == str_grow(&(asm_gen->lines[(asm_gen->len)++]), "\t.file\t\"file.c\""))
		goto clean;
	if (-1 == str_grow(&(asm_gen->lines[(asm_gen->len)++]), "\t.text"))
		goto clean;
	for (size_t i = 0; i < ast->len;i++){
		if (-1 == asm_gen_func(asm_gen, ast->nodes[i]))
			goto clean;
	}
	if (-1 == asm_add_line(asm_gen, "\t.section\t.note.GNU-stack,\"\",@progbits"))
		goto clean;
	return asm_gen;
clean:
	free_asm_gen(asm_gen);
	return NULL;
}