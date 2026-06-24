#include "micro_cc.h"
#include <stdio.h>
#include <assert.h>

static void usage(char *format, char *prog_name){
	printf(format, prog_name);
	exit(EXIT_FAILURE);
}

void free_token(void *a){
	if (!a)
		return;
	token_t * tok = (token_t*)a;
	free(tok->value);
	free(tok);
}
int main(int argc, char *argv[]){
	if (argc == 1)
		usage("%s [file.c]+\n", argv[0]);
	array_t *lex = arr_init(1);
	assert(lex && "lex failed alloc");
	for (unsigned int i = 1; argv[i]; i++){
		if (*argv[i] == '-')
			continue;
		FILE * f = fopen(argv[i], "r");
		if (!f){
			fprintf(stderr, "Failed to open %s\n", argv[i]);
			continue;
		}
		char *lineptr = NULL;
		size_t linesiz = 0;
		while(1){
			if (getline(&lineptr, &linesiz, f) == -1)
				break;
			if (!linesiz)
				break;
			lexer(&lex, lineptr);
			free(lineptr);
			lineptr = NULL;
		}
		free(lineptr);
		fclose(f);
	}
	print_toks(lex);
	arr_free(lex, free_token);
}