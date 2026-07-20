#include "parser.h"
#include <stdio.h>

void print_exp_node(struct exp_node *exp_node, int level){
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	if (!exp_node){
		fprintf(stderr, "NULL EXPR NODE");
		return;
	}
	fprintf(stderr, "(CONST %d\n", exp_node->constant);
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	fprintf(stderr, ")\n");
}

void print_ret_node(struct ret_node *ret_node, int level){
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	if (!ret_node){
		fprintf(stderr, "NULL RETURN NODE");
		return;
	}
	fprintf(stderr, "(RETURN \n");
	print_exp_node(ret_node->node, level+1);
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	fprintf(stderr, ")\n");
}

void print_stmt_node(struct stmt_node *stmt_node, int level){
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	if (!stmt_node){
		fprintf(stderr, "NULL STATEMENT NODE");
		return;
	}
	fprintf(stderr, "(STMT \n");
	print_ret_node(stmt_node->node, level+1);
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	fprintf(stderr, ")\n");
}

void print_func_node(struct func_node *func_node, int level){
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	if (!func_node){
		fprintf(stderr, "NULL FUNCTION NODE");
		return;
	}
	fprintf(stderr, "(Function %s\n", func_node->name);
	print_stmt_node(func_node->node, level+1);
	for (int i = 0; i < level; i++)
		fputc('\t', stderr);
	fprintf(stderr, ")\n");
}
void print_ast(struct program_node *ast){
	fprintf(stderr, "(Build-Unit\n");
	print_func_node(ast->node, 1);
	fprintf(stderr, ")\n");
}