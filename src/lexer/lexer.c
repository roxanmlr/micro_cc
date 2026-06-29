#include "lexer.h"
#include <ctype.h>
#include <assert.h>
#include <stdio.h>

#define CONST_NUM_ERROR 1
#define OUTSIDE_ALPHABET 2
static int lex_err = 0;

static ssize_t lex_id(char *src)
{
	ssize_t n = 0;
	if (!isalpha(*src))
		return -1;
	while (isalnum(src[n]) || src[n] == '_')
		n++;
	return n;
}

static ssize_t lex_const_num(char *src, int *p_lex_err)
{
	ssize_t n = 0;
	if (!isdigit(*src))
		return -1;
	while (isdigit(src[n]))
		n++;
	if (isalpha(src[n]) || src[n] == '_') {
		if (p_lex_err)
			*p_lex_err = CONST_NUM_ERROR;
		fprintf(stderr,
			"Unexpected character `%c` when expecting a digit on constant number `%.*s`\n",
			src[n], (int)(n + 1), src);
		return -1;
	}
	return n;
}

static void free_tok(void *ptr)
{
	token_t *tok = (token_t *) ptr;
	free(tok->value);
	free(tok);
}

ssize_t lexer(array_t **p_lex, char *src)
{
	if (!p_lex || !*p_lex || !src)
		return -1;
	array_t *lex = *p_lex;
	ssize_t n = -1;
	size_t tok_lex = 0;
	while (*src) {
		while (isspace(*src))
			src++;
		if (!(*src))
			break;
		token_t *tok = calloc(1, sizeof(token_t));
		if ('0' <= *src && *src <= '9') {
			n = lex_const_num(src, &lex_err);
			if (lex_err == CONST_NUM_ERROR) {
				free(tok);
				arr_free(*p_lex, free_tok);
				*p_lex = NULL;
				return -1;
			}
			tok->value = strndup(src, n);
			tok->name = TOK_CONSTANT;
			src += n;
		} else if ((n = lex_id(src)) != -1) {
			tok->value = strndup(src, n);
			if (!strcmp(tok->value, "int"))
				tok->name = TOK_KW_INT;
			else if (!strcmp(tok->value, "return"))
				tok->name = TOK_KW_RETURN;
			else if (!strcmp(tok->value, "void"))
				tok->name = TOK_KW_VOID;
			else
				tok->name = TOK_IDENTIFIER;
			src += n;
		} else if (strchr(";{}()", *src)) {
			if (*src == ';')
				tok->name = TOK_SEM_COLON;
			else if (*src == '{')
				tok->name = TOK_BRACE_LEFT;
			else if (*src == '}')
				tok->name = TOK_BRACE_RIGHT;
			else if (*src == '(')
				tok->name = TOK_PAREN_LEFT;
			else if (*src == ')')
				tok->name = TOK_PAREN_RIGHT;
			src++;
		} else {
			fprintf(stderr, "Unexpected character `%c`\n", *src);
			free(tok);
			arr_free(*p_lex, free_tok);
			*p_lex = NULL;
			return -1;
		}
		if (!arr_append(lex, (void *)tok)) {
			assert(lex && "lex is null");
			arr_grow(p_lex, 2048);
			lex = *p_lex;
			arr_append(lex, (void *)tok);
			tok_lex++;
		}
	}
	return tok_lex;
}

int print_toks(array_t *lex)
{
	if (!lex)
		return -1;
	int w = 0;
	int tabs = 0;
	(void)tabs;
	for (unsigned int i = 0; i < lex->len; i++) {
		token_t *tok = (lex->data)[i];
		switch (tok->name) {
		case TOK_IDENTIFIER:
			w += printf("[IDENTIFIER:%s]", tok->value);
			break;
		case TOK_CONSTANT:
			w += printf("[CONSTANT:%s]", tok->value);
			break;
		case TOK_KW_RETURN:
		case TOK_KW_VOID:
		case TOK_KW_INT:
			w += printf(" %s ", tok->value);
			break;
		case TOK_PAREN_LEFT:
			w += printf("(");
			break;
		case TOK_PAREN_RIGHT:
			w += printf(")");
			break;
		case TOK_BRACE_LEFT:
			w += printf("{\n");
			tabs++;
			tabs++;
			break;
		case TOK_BRACE_RIGHT:
			w += printf("}\n");
			tabs--;
			break;
		case TOK_SEM_COLON:
			w += printf(" ;\n");
			break;
		default:
			printf("[Not printable token]");
			break;
		}
	}
	return w;
}
