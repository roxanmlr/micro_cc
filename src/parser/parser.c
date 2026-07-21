#include "parser.h"
#include <stdio.h>
#include <string.h>

static void free_exp_node(struct exp_node *exp_node){
	free(exp_node);
}

static void free_ret_node(struct ret_node *ret_node){
	if (!ret_node)
		return;
	free_exp_node(ret_node->node);
	free(ret_node);
}

static void free_stmt_node(struct stmt_node *stmt_node){
	if (!stmt_node)
		return ;
	free_ret_node(stmt_node->node);
	free(stmt_node);
}

static void free_func_node(struct func_node *func_node){
	if (!func_node)
		return ;
	free(func_node->name);
	free_stmt_node(func_node->node);
	free(func_node);
}

void free_ast(struct program_node *prg){
	if (!prg)
		return ;
	for (size_t i = 0; i < prg->len; i++)
		free_func_node(prg->nodes[i]);
	free(prg->nodes);
	free(prg);
}


#define EXPECTED_A_TOKEN_GOT_NONE 1
#define EXPECTED_A_TOKEN_GOT_ANOTHER 2
//static enum token_name other_token = 0;
static void info_try_consume(int err, enum token_name expected, enum token_name got){
	if (!err)
		return ;
	if (err == EXPECTED_A_TOKEN_GOT_NONE)
		fprintf(stderr, "Expected a %s got none\n", lex_token_name(expected));
	if (err == EXPECTED_A_TOKEN_GOT_ANOTHER)
		fprintf(stderr, "Expected a %s got %s\n", lex_token_name(expected), lex_token_name(got));
}

static token_t * try_consume(array_t *lex, size_t pos, enum token_name tok_name, int *err, enum token_name *other_token){
	if (!lex || !other_token || lex->len <= pos){
		if (err)
			*err = EXPECTED_A_TOKEN_GOT_NONE;
		return NULL;
	}
	token_t * token = (token_t *)(lex->data[pos]);
	if (!token){
		if (err)
			*err = EXPECTED_A_TOKEN_GOT_NONE;
		return NULL;
	}
	if (token->name != tok_name){
		if (err)
			*err = EXPECTED_A_TOKEN_GOT_ANOTHER;
		*other_token = token->name;
		return NULL;
	}
	/*fprintf(stderr, "Consumed %s\n", lex_token_name(tok_name));*/
	return token;
}

/*
	<exp> ::= constant_int
*/
static struct exp_node *parse_exp_node(array_t *lex,  size_t *end_pos){
	if (!lex || !end_pos || lex->len < *end_pos)
		return NULL;
	struct exp_node * exp_node = calloc(1, sizeof(struct exp_node));
	if (!exp_node){
		perror("calloc");
		return NULL;
	}
	size_t pos = *end_pos;
	int err = 0;
	enum token_name tok_expected = TOK_CONSTANT;
	enum token_name other_token = 0;
	token_t *token = try_consume(lex, pos, tok_expected, &err, &other_token);
	if (!token)
		goto error;
	exp_node->constant = atoi(token->value);
	return exp_node;
error:
	info_try_consume(err, tok_expected, other_token);
	free_exp_node(exp_node);
	return NULL;
}

/*
	<return_exp> ::= "return" <instruction>
*/
static struct ret_node *parse_ret_node(array_t *lex,  size_t *end_pos){
	if (!lex || !end_pos || lex->len < *end_pos)
		return NULL;
	struct ret_node *ret_node = calloc(1, sizeof(struct ret_node));
	size_t pos = *end_pos;
	int err = 0;
	enum token_name tok_expected = TOK_KW_RETURN;
	enum token_name other_token = 0;
	if (!try_consume(lex, pos, tok_expected, &err, &other_token))
		goto error;
	pos++;
	ret_node->node = parse_exp_node(lex, &pos);
	if (!ret_node->node){
		fprintf(stderr, "Expected a expression statement\n");
		goto clean;
	}
	tok_expected = TOK_SEM_COLON;
	if(!try_consume(lex, ++pos, tok_expected, &err, &other_token))
		goto error;
	*end_pos = ++pos;
	return ret_node;
error:
	info_try_consume(err, tok_expected, other_token);
clean:
	free_ret_node(ret_node);
	return NULL;
}

/*
	<statement> ::= <return_exp>
*/
static struct stmt_node * parse_stmt_node(array_t *lex,  size_t *end_pos){
	if (!lex || !end_pos || lex->len < *end_pos)
		return NULL;
	struct stmt_node * stmt_node = calloc(1, sizeof(struct stmt_node));
	if (!stmt_node){
		perror("calloc");
		return NULL;
	}
	stmt_node->node = parse_ret_node(lex, end_pos);
	if (!stmt_node->node)
		return free_stmt_node(stmt_node), NULL;
	return stmt_node;
}

/*
	<function> ::= "int" <identifier> "(" "void" ")" "{" <statement> "}"
*/
static struct func_node * parse_func_node(array_t *lex,  size_t *end_pos){
	if (!lex || !end_pos || lex->len < *end_pos)
		return NULL;
	struct func_node *func_node = calloc(1, sizeof(struct func_node));
	if (!func_node){
		perror("calloc");
		return NULL;
	}
	/*Take the function return type*/
	int err = 0;
	size_t pos = *end_pos;
	enum token_name tok_expected = TOK_KW_INT;
	enum token_name other_token = 0;
	if (!try_consume(lex, pos, tok_expected, &err, &other_token))
		goto error;
	/*Check it */
	tok_expected = TOK_IDENTIFIER;
	token_t * token = try_consume(lex, ++pos, tok_expected, &err, &other_token);
	if (!token)
		goto error;
	func_node->name = strdup(token->value);
	if (!func_node->name){
		perror("strdup");
		goto clean;
	}
	tok_expected = TOK_PAREN_LEFT;
	if (!try_consume(lex, ++pos, tok_expected, &err, &other_token))
		goto error;
	tok_expected = TOK_KW_VOID;
	if (!try_consume(lex, ++pos, tok_expected, &err, &other_token))
		goto error;
	tok_expected = TOK_PAREN_RIGHT;
	if (!try_consume(lex, ++pos, tok_expected, &err, &other_token))
		goto error;
	tok_expected = TOK_BRACE_LEFT;
	if (!try_consume(lex, ++pos, tok_expected, &err, &other_token))
		goto error;
	pos++;
	func_node->node = parse_stmt_node(lex, &pos);
	if (!func_node->node)
		goto error;
	tok_expected = TOK_BRACE_RIGHT;
	if (!try_consume(lex, pos, tok_expected, &err, &other_token))
		goto error;
	*end_pos = ++pos;
	return func_node;
error:
	info_try_consume(err, tok_expected, other_token);
clean:
	free_func_node(func_node);
	return NULL;
}

/*
 <program> ::= <function>
*/
struct program_node *parse(array_t *lex){
	if (!lex && !lex->len)
		return NULL;
	struct program_node *prg_node = calloc(1, sizeof(struct program_node));
	if (!prg_node){
		perror("calloc");
		return NULL;
	}
	size_t end_pos = 0;
	prg_node->cap = 32;
	prg_node->nodes = (struct func_node **) calloc(prg_node->cap, sizeof(struct func_node *));
	if (!prg_node->nodes){
		perror("calloc");
		free(prg_node);
		return NULL;
	}
	while(end_pos < lex->len){
		(prg_node->nodes)[prg_node->len] = parse_func_node(lex, &end_pos);
		if (!(prg_node->nodes)[prg_node->len])
			return free_ast(prg_node), NULL;
		(prg_node->len)++;
		if (prg_node->len == prg_node->cap){
			prg_node->cap *= 2;
			struct func_node ** func_nodes = (struct func_node **) calloc(prg_node->cap, sizeof(struct func_node *));
			memmove(func_nodes, prg_node->nodes, (prg_node->cap / 2) * sizeof(struct func_node *));
			free(prg_node->nodes);
			prg_node->nodes = func_nodes;
		}
	}
	return prg_node;
}
