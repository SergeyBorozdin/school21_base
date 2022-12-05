#define _GNU_SOURSE  // getline

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int regeg_flag;
  // bool e;  // шаблон (ищем это в каждой строчке файла) grep -e "ev" 123.txt
  // -e
  //  "one"
  // bool i;  // игнорирует регистр
  bool v;  // выводит строки которые не содержат grep -v "e" 123.txt
  bool c;  // выводит количество совпадений
  bool l;  // выводит имя файла где есть совпадение
  bool n;  // нумерует строки
  // bool h;  // выводит строки в множестве файлов без названия фалов
  // bool s;  // не выводит grep: 1243.txt: No such file or directory
  // bool f;  // file  получает регулярные выражения из файла
  // bool o;  // печатает только совпадающие символы
} Flags;

Flags grep_read_flags(int argc, char *argv[]) {
  Flags flags = {0, false, false, false, false};
  int current_flag = getopt(argc, argv, "eivcln");

  for (; current_flag != -1; current_flag = getopt(argc, argv, "eivcln")) {
    switch (current_flag) {
      case 'e':
        flags.regeg_flag |= REG_EXTENDED;
        break;
      case 'i':
        flags.regeg_flag |= REG_ICASE;
        break;
      case 'v':
        flags.v = true;
        break;
      case 'c':
        flags.c = true;
        break;
      case 'l':
        flags.l = true;
        break;
      case 'n':
        flags.n = true;
        break;
    }
  }
}

void grep_file(FILE *file, Flags flags, regex_t *preg) {
  (void)flags;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (flags.v) {
      if (regexec(preg, line, 1, &match, 0)) {
        printf("%s", line);
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        printf("%s", line);
      }
    }
  }
  free(line);
}

void grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  regex_t preg_storage;
  regex_t *preg = &preg_storage;

  for (; pattern != end && pattern[0][0] == '-'; ++pattern)
    ;
  if (pattern == end) {
    fprintf(stderr, "no pattern\n");
    exit(1);
  }

  if (regcomp(preg, *pattern, flags.regeg_flag)) {
    fprintf(stderr, "failed to cimple regex\n");
    exit(1);
  }

  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') {
      continue;
    }
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    grep_file(file, flags, preg);
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = grep_read_flags(argc, argv);
  // debug
  // https://www.youtube.com/watch?v=y69SulDLPs8&list=PLhHwymilvEHB2JBlvMNEqHxPMee0e5BDd&index=1&t=19s
  printf("flag v = %d\n", flags.v);
  printf("flag c = %d\n", flags.c);
  printf("flag l = %d\n", flags.l);
  printf("flag n  = %d\n", flags.n);
  printf("regeg_flag =  %d\n", flags.regeg_flag);

  grep(argc, argv, flags);

  return 0;
}
