#include<stdio.h>
#include<string.h>
#include <unistd.h>

#define _GNU_SOURCE
#include <stdlib.h>

typedef struct options {
	int b;
	int e;
	int n;
	int s;
	int t;
	int v;
} opt;



void cat_not_arg() {
    char symbol;
    while(scanf("%c", &symbol)) {
        printf("%c", symbol);
    }
}

void cat_yes_args(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        FILE *stream;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        stream = fopen(argv[i], "r");
        if (stream == NULL)
            exit(EXIT_FAILURE);// функция обработки аргументов
        while ((read = getline(&line, &len, stream)) != -1) {
            //printf("Получена строка длиной %zu :\n", read); // здесь ветвление чтениние или чтение по флагам
            printf("%s", line);
        }
        free(line);
        fclose(stream);
        exit(EXIT_SUCCESS);

        
        // int length = strlen(argv[i]);
        // for (int j = 0; j < length; j ++) {
        //     printf("%c",argv[i][j]);
        // }
        //printf("%s\n", argv[i]);
    }
}


int main (int argc, char *argv[]) {
    if (argc == 1) {
        cat_not_arg();
    }
    if (argc > 1) {
        cat_yes_args(argc, argv);
    }
    return 0;
}
