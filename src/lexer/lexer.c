#include "lexer.h"
#include <ctype.h>
#include <assert.h>
#include <stdio.h>

#define CONST_NUM_ERROR 1
#define OUTSIDE_ALPHABET 2
static int lex_err = 0;

static ssize_t lex_id(char *src){
	ssize_t n = 0;
	if (!isalpha(*src))
		return -1;
	while(isalnum(src[n]) || src[n] == '_')
		n++;
	return n;
}

static ssize_t lex_const_num(char *src){
	ssize_t n = 0;
	if (!isdigit(*src))
		return -1;
	while(isdigit(src[n]))
		n++;
	if (isalpha(src[n]) || src[n] == '_'){
		lex_err = CONST_NUM_ERROR;
		return -1;
	}
	return n;
}

ssize_t lexer(array_t **p_lex, char *src){
	if (!p_lex || !*p_lex || !src)
		return -1;
	array_t *lex = *p_lex;
	ssize_t n = -1;
	while (*src){
		while(isspace(*src))
			src++;
		if (!(*src))
			break;
		token_t* tok = calloc(1, sizeof(token_t));
		if ((n = lex_id(src)) != -1){
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
		}		
		else if ((n = lex_const_num(src)) != -1){
			tok->value = strndup(src, n);
			tok->name = TOK_CONSTANT;
			src += n;
			
		}
		else if (*src == ';'){
			n = 1;
			src++;
			tok->name = TOK_SEM_COLON;
		}
		else if (*src == '{'){
			n = 1;
			src++;
			tok->name = TOK_BRACE_LEFT;
		}
		else if (*src == '}'){
			n = 1;
			src++;
			tok->name = TOK_BRACE_RIGHT;
		}
		else if (*src == '('){
			n = 1;
			src++;
			tok->name = TOK_PAREN_LEFT;
		}
		else if (*src == ')'){
			n = 1;
			src++;
			tok->name = TOK_PAREN_RIGHT;
		}
		if (n == -1) {
			free(tok);
			break;
		}
		if (!arr_append(lex, (void*)tok)){
			assert(lex && "lex is null");
			arr_grow(p_lex, 2048);
			lex =  *p_lex;
			arr_append(lex, (void*)tok);
		}
	}
	return 1;
}

int print_toks(array_t *lex){
	if (!lex)
		return -1;
	int w = 0;
	int tabs = 0;
	for (unsigned int i = 0; i < lex->len; i++){
		token_t *tok = (lex->data)[i];
		for(u_int16_t t = 0; t < tabs; t++)
			printf("\t");
		switch (tok->name){
			case TOK_IDENTIFIER:
				w += printf("[IDENTIFIER:%s]", tok->value);break;
			case TOK_CONSTANT:
				w += printf("[CONSTANT:%s]", tok->value);break;
			case TOK_KW_RETURN:
			case TOK_KW_VOID:
			case TOK_KW_INT:
				w += printf(" %s ", tok->value);break;
			case TOK_PAREN_LEFT:
				w += printf("(");break;
			case TOK_PAREN_RIGHT:
				w += printf(")");break;
			case TOK_BRACE_LEFT:
				w += printf("{\n");tabs++;tabs++;break;
			case TOK_BRACE_RIGHT:
				w += printf("}\n");tabs--;break;
			case TOK_SEM_COLON:
				w += printf(" ;\n");break;
			default:
				printf("[Not printable token]");break;
		}
	}
	return w;
}