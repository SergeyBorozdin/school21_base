//#include "io.h"
#include <stdio.h>
#include <stdlib.h>

// #include <string.h>
#include <unistd.h>
// #include <fcntl.h>
// #include <errno.h>


// # define BUFFER_LEN 4096


// void reader () {
	
// }

typedef struct options {
	int b; //нумерует не пустые строки
	int e; // показывает конец строки
	int n; // нумерует все строки
	int s; // сжимает пустые строки до одной
	int t; // показыввает табы
	int v; // 
} opt;


void parser(int argc, char *argv[], opt *options) {
	int opt = 0;
	int option_index = 0;

	typedef struct option opts[] = {
		{"number-nonblank", 0, 0, 'b'},
		{"show-ends", 0, 0, 'e'},
		{"number", 0, 0, 'n'},
		{"squeeze-blank", 0, 0, 's'},
		{"show-tabs", 0, 0, 't'},
		{"show-nonprinting", 0, 0, 'v'},
		{0, 0, 0, 0},
	};
	
	while((opt = getopt_long(argc, argv, "benstvTE", opts, &option_index)) != -1) {
		switch(opt) {
			case 'b': options->b = 1; break;
			case 'e': options->e = 1;
					  options->v = 1; break;
			case 'n': options->b = 1; break;
			case 's': options->s = 1; break;
			case 't': options->t = 1;
					  options->v = 1; break;
			case 'v': options->v = 1; break;
			case 'E': options->e = 1; break;
			case 'T': options->t = 1;
					  options->v = 1; break;
			default: fprintf(STDIN_FILENO, "usage", "benstvTE");
			exit(1);
		}
	}
}
 
int main(int argc, char *argv[]) {

	opt options = {0, 0, 0, 0, 0, 0};
	printf("%d\n", options.b);
	printf("%d\n", options.e);
	printf("%d\n", options.n);
	printf("%d\n", options.s);
	printf("%d\n", options.t);
	printf("%d\n", options.v);

	parser(argc, argv, &options);

	printf("%d\n", options.b);
	printf("%d\n", options.e);
	printf("%d\n", options.n);
	printf("%d\n", options.s);
	printf("%d\n", options.t);
	printf("%d\n", options.v);

	return 0;
 
}
