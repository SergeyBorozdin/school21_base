#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int e;  // шаблон (ищем это в каждой строчке файла) grep -e "ev" 123.txt -e
          // "one"
  int i;  // игнорирует регистр
  int v;  // выводит строки которые не содержат grep -v "e" 123.txt
  int c;  // выводит количество совпадений
  int l;  // выводит имя файла где есть совпадение
  int n;  // нумерует строки
  int h;  // выводит строки в множестве файлов без названия фалов
  int s;  // не выводит grep: 1243.txt: No such file or directory
  int f;  // file  получает регулярные выражения из файла
  int o;  // печатает только совпадающие символы
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
         (opt = getopt_long(argc, argv, "e:ivclnhsf:o", opts, &option_index))) {
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

int match(char *string, char *pattern, regex_t *re) {
  int status;

  if ((status = regcomp(re, pattern, REG_EXTENDED)) != 0) return (status);
  status = regexec(re, string, 0, NULL, 0);
  return (status);
}

void reader_no_flags(int argc, char *argv[]) {
  char *str_pattern = argv[1];
  int retval;
  regex_t re;

  for (int i = 2; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");

    if (fp != NULL) {
      char line[4096];
      while ((fgets(line, sizeof(line), fp)) != NULL) {
        retval = match(line, str_pattern, &re);
        if (retval == 0) {
          if (argc > 3) {
            printf("%s:%s", argv[i], line);
          } else {
            printf("%s", line);
          }
        }
      }
    } else {
      printf("%s: %s: No such file or directory\n", argv[0], argv[i]);
    }
  }
}

void reader_whith_flags(int argc, char *argv[], opt options) {
  if (options.e) {
    int count_pattern = 0;
    for (int i = 0; i < argc; i++) {
      if (*argv[i] == '-') {
        count_pattern++;
      }
    }

    char *patterns[count_pattern];
    int j = 0;
    for (int i = 0; i != argc; i++) {
      if (*argv[i] == '-') {
        patterns[j] = argv[i + 1];
        j++;
      }
    }

    int retval;
    regex_t re;
    int y = 0;
    int k = 1;

    for (k = k + count_pattern * 2; k < argc; k++) {
      FILE *fp = fopen(argv[k], "r");

      if (fp != NULL) {
        char line[4096];
        while ((fgets(line, sizeof(line), fp)) != NULL) {
          retval = match(line, patterns[y], &re);
          if (retval == 0) {
            if (argc > 3) {
              printf("%s:%s", argv[k], line);
            } else {
              printf("%s", line);
            }
          }
        }
        y++;
      } else {
        printf("%s: %s: No such file or directory\n", argv[0], argv[k]);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    grep_not_arg();
  }

  if (argc > 1) {
    opt options = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    parser(argc, argv, &options);
    if (!options.c && !options.e && !options.f && !options.h && !options.i &&
        !options.l && !options.n && !options.o && !options.s && !options.v) {
      reader_no_flags(argc, argv);
    } else {
      reader_whith_flags(argc, argv, options);
    }
  }

  return 0;
}
