#define _GNU_SOURSE  // getline

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int regeg_flag; // -e шаблон, -i CaMelCaSe, "none flags"
  int v;  // выводит строки которые не содержат grep -v "e" 123.txt
  int c;  // выводит количество совпадений
  int l;  // выводит имя файла где есть совпадение
  int n;  // нумерует строки
  // bool h;  // выводит строки в множестве файлов без названия фалов
  // bool s;  // не выводит grep: 1243.txt: No such file or directory
  // bool f;  // file  получает регулярные выражения из файла
  int o;  // печатает только совпадающие символы
} Flags;

Flags grep_read_flags(int argc, char *argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0};
  int current_flag = getopt(argc, argv, "eivclno");

  for (; current_flag != -1; current_flag = getopt(argc, argv, "eivclno")) {
    switch (current_flag) {
      case 'e':
        flags.regeg_flag |= REG_EXTENDED;
        break;
      case 'i':
        flags.regeg_flag |= REG_ICASE;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'c':
        flags.c = 1;
        break;
      case 'l':
        flags.l = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 'o':
        flags.o = 1;
        break;
    }
  }
  return flags;
}

void grep_count(FILE *file, char *filename, Flags flags, regex_t *preg,
                int count_file) {
  (void)flags;
  (void)filename;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count_match = 0;

  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      count_match++;
    }
  }
  if (count_file == 1) {
    printf("%d\n", count_match);
  } else {
    printf("%s:%d\n", filename, count_match);
  }

  free(line);
}

void grep_number_line(FILE *file, char *filename, Flags flags, regex_t *preg,
                      int count_file) {
  (void)flags;
  (void)filename;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count_line = 1;

  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      if (count_file == 1) {
        printf("%d:%s", count_line, line);
      } else {
        printf("%s:%d:%s", filename, count_line, line);
      }
    }
    ++count_line;
  }
  free(line);
}

void grep_file_name(FILE *file, char *filename, Flags flags, regex_t *preg,
                int count_file) {
  (void)flags;
  (void)filename;
  (void)count_file;
  (void)preg;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;


  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      printf("%s\n", filename);
    }
  }
  free(line);
}

void grep_file(FILE *file, char *filename, Flags flags, regex_t *preg,
               int count_file) {
  (void)flags;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (flags.v) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (flags.o)
          ;
        else
          printf("%s", line);
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (flags.o) {
          if (count_file == 1) {
            printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                   line + match.rm_so);
          } else {
            printf("%s:%.*s\n", filename, (int)(match.rm_eo - match.rm_so),
                   line + match.rm_so);
          }
          char *remaining = line + match.rm_eo;

          while (!regexec(preg, remaining, 1, &match, 0)) { // отладка бесконечного цикла
            printf("%.*s\n", (int)(match.rm_eo - match.rm_so), remaining + match.rm_so);
            remaining = remaining + match.rm_eo;
          }
        } else {
          if (count_file == 1) {
            printf("%s", line);
          } else {
            printf("%s:%s", filename, line);
          }
        }
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
  int count_file = 0;

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
    ++count_file;
    if (count_file >= 2) {
      break;
    }
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
    if (flags.l) {
      grep_file_name(file, *filename, flags, preg, count_file);
    }
    if (flags.n) {
      grep_number_line(file, *filename, flags, preg, count_file);
    }
    if (flags.c) {
      grep_count(file, *filename, flags, preg, count_file);
    } else {
      grep_file(file, *filename, flags, preg, count_file);
    }
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = grep_read_flags(argc, argv);
  grep(argc, argv, flags);

  return 0;
}
