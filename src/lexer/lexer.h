#include "../array/array.h"
#include <sys/types.h>

enum token_name{
	TOK_IDENTIFIER = 1,
	TOK_CONSTANT,
	TOK_KW = 10,
	TOK_KW_INT,
	TOK_KW_VOID,
	TOK_KW_RETURN,
	TOK_PUNCT = 100,
	TOK_PAREN_LEFT,
	TOK_PAREN_RIGHT,
	TOK_BRACE_LEFT,
	TOK_BRACE_RIGHT,
	TOK_SEM_COLON
};

struct token{
	enum token_name name;
	char *value;
	size_t line;
	size_t col;
};

typedef struct token token_t;
/*
lexer take an array of tokens and appends new tokens get from src

Returns:
	on error : -1, allocation failed or bad syntax
	on success : number of token appended 
*/
ssize_t lexer(array_t **lex, char *src);

int print_toks(array_t *lex);