#include "lexer.h"

const char * lex_token_name(enum token_name token_name){
	switch (token_name) {
		case TOK_IDENTIFIER: return "an identifier";
		case TOK_CONSTANT: return "a constant";
		case TOK_KW : return "a keyword";
		case TOK_KW_INT: return "the 'int' keyword";
		case TOK_KW_VOID: return "the 'void' keyword";
		case TOK_KW_RETURN: return "the 'return' keyword";
		case TOK_PUNCT: return "a punctuation";
		case TOK_PAREN_LEFT: return "a left parenthese '('";
		case TOK_PAREN_RIGHT: return "a right parenthese ')'";
		case TOK_BRACE_LEFT: return "a left brace '{'";
		case TOK_BRACE_RIGHT: return "a right brace '}'";
		case TOK_SEM_COLON: return "a semicolon ';'";
	}
}