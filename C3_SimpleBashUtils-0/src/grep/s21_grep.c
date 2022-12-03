#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct options {
  int e;  // шаблон
  int i;  // игнорирует регистр
  int v;  // инвертирует смысл поиска соответсвий
  int c;  // выводит только количесвто совпадающих строк
  int l;  // выводит только совпадающие файлы
  int n;  // предваряет каждую строку вывода номером строки из файла ввода
  int h;  // выводит совпадающие строки не предваряя их именам файла
  int s;  // подавляет сообщения об ошибках о несуществующих или нечитаемых
          // файлах
  int f;  // file  получает регулярные выражения из файла
  int o;  // печатает только совпадающие(не пустые) части совпавшей строки
} opt;

void grep_not_arg() {
  char symbol;
  while (scanf("%c", &symbol)) {
    printf("%c", symbol);
  }
}

void parser(int argc, char *argv[], opt *options) {
  int opt = 0;
  int option_index = 0;
  struct option opts[] = {
      {"pattern", 0, 0, 'e'},
      {"ignore-lowercase", 0, 0, 'i'},
      {"invert-match", 0, 0, 'v'},
      {"count-matching-lines", 0, 0, 'c'},
      {"matching-files", 0, 0, 'l'},
      {"line-number", 0, 0, 'n'},
      {"matching-lines-without-file-names", 0, 0, 'h'},
      {"show-ends", 0, 0, 's'},
      {"number", 0, 0, 'n'},
      {"messages-nonexistent-files.", 0, 0, 's'},
      {"take-regexes-file", 0, 0, 'f'},
      {"print-parts-matching-line", 0, 0, 'o'},
      {0, 0, 0, 0},
  };

  while (-1 !=
         (opt = getopt_long(argc, argv, "+eivclnhsfo", opts, &option_index))) {
    switch (opt) {
      case 'e':
        options->e = 1;
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        fprintf(stderr, "usage - 'eivclnhsfo'");
        exit(1);
    }
  }
}

// void reader(int argc, char *argv[], opt options) {
//   for (int i = 1; i < argc; i++) {
//     if (*argv[i] == '-') {
//       continue;
//     } else {
//       FILE *fp = fopen(argv[i], "r");

//       if (fp != NULL) {
//         int symbol = 0;
//         int symbol_prev = '\n';
//         int flag_bool = 0;
//         int string_count = 0;
//         int empty_string = 0;

//         while ((symbol = fgetc(fp)) != EOF) {
//           if (options.s == 1 && symbol == '\n' && symbol_prev == '\n') {
//             empty_string++;
//             if (empty_string > 1) {
//               flag_bool = 1;
//             }
//           } else {
//             flag_bool = 0;
//             empty_string = 0;
//           }
//           if (flag_bool == 0) {
//             if (((options.n == 1 && options.b == 0) ||
//                  (options.b == 1 && symbol != '\n')) &&
//                 symbol_prev == '\n') {
//               printf("%*d\t", 6, ++string_count);
//             }
//             if (options.t == 1 && symbol == '\t') {
//               printf("^");
//               symbol = 'I';
//             }
//             if (options.e == 1 && symbol == '\n') {
//               printf("$");
//             }
//             if (options.v == 1) {
//               if ((symbol >= 0 && symbol < 9) || (symbol > 10 && symbol <
//               32)) {
//                 printf("^");
//                 symbol = symbol + 64;
//               } else if (symbol == 127) {
//                 printf("^");
//                 symbol = '?';
//               }
//             }
//             printf("%c", symbol);
//             symbol_prev = symbol;
//           }
//         }
//       } else {
//         printf("%s: %s: No such file or directory\n", argv[0], argv[i]);
//       }
//     }
//   }
// }

int main(int argc, char *argv[]) {
  if (argc == 1) {
    grep_not_arg();
  }

  opt options = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  parser(argc, argv, &options);

  printf("options.e - %d\n", options.e);
  printf("options.i - %d\n", options.i);
  printf("options.v - %d\n", options.v);
  printf("options.c - %d\n", options.c);
  printf("options.l - %d\n", options.l);
  printf("options.n - %d\n", options.n);
  printf("options.h - %d\n", options.h);
  printf("options.s - %d\n", options.s);
  printf("options.f - %d\n", options.f);
  printf("options.o - %d\n", options.o);

  // reader(argc, argv, options);

  return 0;
}
