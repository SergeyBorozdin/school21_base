#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct options {
  int b;  //нумерует не пустые строки
  int e;  // показывает конец строки
  int n;  // нумерует все строки
  int s;  // сжимает пустые строки до одной
  int t;  // показыввает табы
  int v;  // показывает не читаемые символы
} opt;

// char const *print_unprintable(int c) {
// 	char const *code[32] = {"^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G",
// 	"^H", " ", "\v", "^K", "^L", "^M", "^N", "^O", "^P", "^Q", "^R", "^S",
// 	"^T", "^U", "^V", "^W", "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^",
// "^_"};

// 	return (code[c]);
// }

void cat_not_arg() {
  char symbol;
  while (scanf("%c", &symbol)) {
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

  while (-1 !=
         (opt = getopt_long(argc, argv, "+benstvTE", opts, &option_index))) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        break;
      case 'v':
        options->v = 1;
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      default:
        fprintf(stderr, "usage - 'benstvTE'");
        exit(1);
    }
  }
}

// void print_no_flag(FILE *file) {

// 	char symbol;
// 	while ((symbol=fgetc(file))!=EOF) {
//         printf("%c", symbol);
//     }
// }

// void flag_b(FILE *file) { //  нумерует не пустые строки

//     int flag_bool = 0;
//     int str_count = 0;
// 	char symbol;
// 	while ((symbol=fgetc(file))!=EOF){
//         if (symbol != '\n'){
//             if (flag_bool == 0) {
//                 printf("%6d\t", ++str_count);
//                 flag_bool = 1;
//             }
//         } else {
//             flag_bool = 0;
//         }
//         printf("%c", symbol);
//     }
// }

// void flag_e(FILE *file) { // показывает конец строки

//     char symbol;
// 	while ((symbol=fgetc(file))!=EOF) {
//         if (symbol == '\n'){
//             printf("%c%c", '$', '\n');
//             symbol++;
//         } else if (symbol == '\0') {
//             printf("%c", '$');
//         } else {
//             printf("%c", symbol);
//         }
//     }
// }

// void flag_n(FILE *file) { // нумерует все строки

//     int flag_bool = 0;
//     int str_count = 0;
//     size_t symbol_count = 0;
// 	char symbol;
// 	while ((symbol=fgetc(file))!=EOF){
//         if (symbol != '\n' || (symbol == '\n' && symbol_count == '\n')){
//             if (flag_bool == 0) {
//                 printf("%6d\t", ++str_count);
//                 flag_bool = 1;
//             }
//         } else {
//             flag_bool = 0;
//         }
//         printf("%c", symbol);
//         symbol_count++;
//     }
// }

// void flag_s(FILE *file) { // сжимает пустые строки до одной

//     char symbol;
// 	while ((symbol=fgetc(file))!=EOF){
//         printf("%c", symbol);
//     }
// }

// void flagt(FILE *file) { // показыввает табы

//     char symbol;
// 	while ((symbol=fgetc(file))!=EOF){
//         printf("%c", symbol);
//     }
// }

// void flag_v(FILE *file) { // показывает не читаемые символы

// 	char symbol;
// 	while ((symbol=fgetc(file))!=EOF){
// 		printf("%c", symbol);

//     }
// }

// int counter_sumbol_file (FILE *file) { // показывает не читаемые символы

// 	char symbol;
// 	int counter = 0;
// 	while ((symbol=fgetc(file))!=EOF) {
// 		counter++;
//     }
// 	return counter * 0.1;
// }

void reader(int argc, char *argv[], opt options) {
  for (int i = 1; i < argc; i++) {
    if (*argv[i] == '-') {
      continue;
    } else {
      FILE *fp = fopen(argv[i], "r");

      if (fp != NULL) {
        int flag_bool = 0, symbol = 0, symbol_prev = 0, flag_bool_s = 0,
            str_count = 0, i = 0;

        while ((symbol = fgetc(fp)) != EOF) {
          if (options.b == 1) {
            if (symbol != '\n') {
              if (flag_bool == 0) {
                printf("%6d\t", ++str_count);
                flag_bool = 1;
              }
            } else {
              flag_bool = 0;
            }
          }
          if (options.e == 1) {
            if (symbol == '\n') {
              printf("%c", '$');
            }
          }
          if (options.n == 1) {
            if (i == 0) {
              printf("%6d\t", ++str_count);
            }
            if (symbol_prev == '\n') {
              printf("%6d\t", ++str_count);
            }
          }
          if (options.s == 1) {
            if (symbol_prev == '\n' && symbol == '\n' && flag_bool_s == 0) {
              flag_bool_s = 1;
              continue;
            } else {
              flag_bool_s = 0;
            }
          }
          if (options.t == 1) {
            if (symbol == '\t') {
              printf("%c%c", '^', 'I');
              continue;
            }
          }
          if (options.v == 1) {
            if ((symbol >= 0 && symbol < 9) || (symbol > 10 && symbol < 32)) {
              printf("^%c", symbol + 64);
              continue;
            }
            if (symbol == 127) {
              printf("^%c", '?');
              continue;
            }
          }
          // if (options.b == 0 && options.e == 0 && options.n == 0 && options.s
          // == 0 && options.t == 0 && options.v == 0) {
          // print_no_flag(fp);
          // }

          printf("%c", symbol);
          if (flag_bool_s) {
            printf("\n");
          }
          symbol_prev = symbol;
          i++;
        }

      } else {
        printf("%s: %s: No such file or directory\n", argv[0], argv[i]);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cat_not_arg();
  }

  opt options = {0, 0, 0, 0, 0, 0};
  // printf("options.b - %d\n", options.b); printf("options.e - %d\n",
  // options.e); printf("options.n - %d\n", options.n); printf("options.s -
  // %d\n", options.s); printf("options.t - %d\n", options.t); printf("options.v
  // - %d\n", options.v);

  parser(argc, argv, &options);
  // printf("\n"); printf("options.b - %d\n", options.b); printf("options.e -
  // %d\n", options.e); printf("options.n - %d\n", options.n); printf("options.s
  // - %d\n", options.s); printf("options.t - %d\n", options.t);
  // printf("options.v - %d\n", options.v);

  reader(argc, argv, options);

  return 0;
}