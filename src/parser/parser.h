#ifndef PARSER_H
#define PARSER_H
#include "../lexer/lexer.h"

struct exp_node {
	int constant;
};
struct ret_node {
	struct exp_node * node;
};
struct stmt_node {
	struct ret_node *node;
};
struct func_node {
	char *name;
	struct stmt_node *node;
};
struct program_node {
	struct func_node **nodes;
	size_t len;
	size_t cap;
};

/*lex is an array_t of token_t pointers */
struct program_node *parse(array_t *lex);
void free_ast(struct program_node *prg);
void print_ast(struct program_node *ast);
#endif