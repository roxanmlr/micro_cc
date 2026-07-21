#include "parser.h"
#include <stdio.h>
#include <unistd.h>

static inline void putnchar(char c, int n){
	char p[n+1];
	memset(p, c, n);
	p[n] = 0;
	write(STDERR_FILENO,p, n);
}

void print_exp_node(struct exp_node *exp_node){
	if (!exp_node){
		fprintf(stderr, "NULL EXPR NODE");
		return;
	}
	fprintf(stderr, "(CONST %d)", exp_node->constant);
}

void print_ret_node(struct ret_node *ret_node){
	if (!ret_node){
		fprintf(stderr, "NULL RETURN NODE");
		return;
	}
	fprintf(stderr, "(RETURN ");
	print_exp_node(ret_node->node);
	fprintf(stderr, ")");
}

void print_stmt_node(struct stmt_node *stmt_node, int level){
	(void)level;
	print_ret_node(stmt_node->node);
	fprintf(stderr,")\n");
}

void print_func_node(struct func_node *func_node, int level){
	putnchar('\t', level);
	if (!func_node){
		fprintf(stderr, "NULL FUNCTION NODE");
		return;
	}
	fprintf(stderr, "(Function\n");
	putnchar('\t', level + 1);
	fprintf(stderr, "name=\"%s\",\n", func_node->name);
	putnchar('\t', level + 1);
	fprintf(stderr, "body=");
	print_stmt_node(func_node->node, level+1);
	putnchar('\t', level);
	fprintf(stderr, ")\n");
}
void print_ast(struct program_node *ast){
	fprintf(stderr, "(Build-Unit\n");
	for (size_t i = 0; i < ast->len; i++)
		print_func_node((ast->nodes)[i], 1);
	fprintf(stderr, ")\n");
}