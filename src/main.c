#include "micro_cc.h"
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

static void usage(char *format, char *prog_name){
	printf(format, prog_name);
	exit(EXIT_FAILURE);
}

static void sys_exit(char *func, int exit_code){
	perror(func);
	exit(exit_code);
}

void free_token(void *a){
	if (!a)
		return;
	token_t * tok = (token_t*)a;
	free(tok->value);
	free(tok);
}

int cpp_file(char *filename, FILE **in, int *pid){
	if (!filename || !in || !pid)
		return -1;
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
		sys_exit("open", EXIT_FAILURE);
	int pipe_out[2];
	if (pipe(pipe_out) == -1)
		sys_exit("pipe", EXIT_FAILURE);
	*pid = fork();
	if (*pid == -1)
		sys_exit("fork", EXIT_FAILURE);
	if (*pid == 0){
		close(pipe_out[0]);
		if (dup2(fd, STDIN_FILENO) == -1)
			sys_exit("dup2", EXIT_FAILURE);
		if (dup2(pipe_out[1], STDOUT_FILENO) == -1)
			sys_exit("dup2", EXIT_FAILURE);	
		execlp("cpp", "cpp", "-P", NULL);
		sys_exit("execlp", EXIT_FAILURE);
	}
	close(pipe_out[1]);
	close(fd);
	*in = fdopen(pipe_out[0], "r");
	if (!*in)
		sys_exit("fdopen", EXIT_FAILURE);
	return 1;
}

int main(int argc, char *argv[]){
	if (argc == 1)
		usage("%s [file.c]+\n", argv[0]);
	array_t *lex = arr_init(1);
	assert(lex && "lex failed alloc");
	for (unsigned int i = 1; argv[i]; i++){
		if (*argv[i] == '-')
			continue;
		FILE * f = NULL;
		int pid;
		if (cpp_file(argv[1], &f, &pid) == -1)
			continue;
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
		waitpid(pid, NULL, 0);
	}
/*	print_toks(lex);*/
	if (!lex)
		return EXIT_FAILURE;
	struct program_node * ast = parse(lex);
	arr_free(lex, free_token);
	if (!ast)
		return EXIT_FAILURE;
	/*print_ast(ast);*/
	asm_gen_t * _asm = asm_gen(ast);
	free_ast(ast);
	if (!_asm)
		return EXIT_FAILURE;
	print_asm_gen(_asm, stdout);
	free_asm_gen(_asm);
	return EXIT_SUCCESS;
}