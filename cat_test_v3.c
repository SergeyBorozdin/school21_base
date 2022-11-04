#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

typedef struct options {
	int b; //нумерует не пустые строки
	int e; // показывает конец строки
	int n; // нумерует все строки
	int s; // сжимает пустые строки до одной
	int t; // показыввает табы
	int v; // показывает не читаемые символы
} opt;

void cat_not_arg() {
    char symbol;
    while(scanf("%c", &symbol)) {
        printf("%c", symbol);
    }
}

void parser(int argc, char *argv[], opt *options) {
	int opt = 0;
	int option_index = 0;
	struct option opts[] = {
		{"number-nonblank", 0, 0, 'b'},
		{"show-ends", 0, 0, 'e'},
		{"number", 0, 0, 'n'},
		{"squeeze-blank", 0, 0, 's'},
		{"show-tabs", 0, 0, 't'},
		{"show-nonprinting", 0, 0, 'v'},
		{0, 0, 0, 0},
	};
	
	while(-1 != (opt = getopt_long(argc, argv, "+benstvTE", opts, &option_index))) {
		switch(opt) {
			case 'b': options->b = 1; break;
			case 'e': options->e = 1; options->v = 1; break;
			case 'n': options->n = 1; break;
			case 's': options->s = 1; break;
			case 't': options->t = 1; break;
			case 'v': options->v = 1; break;
			case 'E': options->e = 1; options->v = 1; break;
			case 'T': options->t = 1; break;
			default: fprintf(stderr, "usage - 'benstvTE'");
			exit(1);
		}
	}
}


void reader(char *argv[], opt options) {

	FILE *fp = fopen(argv[optind], "r");
    
    if (fp != NULL) {
        int cur;
        int str_count = 0;
        int end_str_count = 1;
        int counter = 0;

        while((cur = fgets(fp)) != EOF) {
            if (options.b) { // вынести отработку  флагов в разные функции
                if (cur != '\n') {
                    if (counter == 0) {
                        printf("%6d\t", ++str_count);
                        counter = 1;
                    }
                } else {
                    counter = 0;
                }
            }

        }

    } else {
        printf("%s: No such file or directory", argv[optind]);
    }
}
 
int main(int argc, char *argv[]) {

	if (argc == 1) {
        cat_not_arg();
    }

	opt options = {0, 0, 0, 0, 0, 0};
	printf("options.b - %d\n", options.b); printf("options.e - %d\n", options.e); printf("options.n - %d\n", options.n); printf("options.s - %d\n", options.s); printf("options.t - %d\n", options.t); printf("options.v - %d\n", options.v);

	parser(argc, argv, &options);
	printf("\n"); printf("options.b - %d\n", options.b); printf("options.e - %d\n", options.e); printf("options.n - %d\n", options.n); printf("options.s - %d\n", options.s); printf("options.t - %d\n", options.t); printf("options.v - %d\n", options.v);

    reader(argv, options);


	return 0;
}
