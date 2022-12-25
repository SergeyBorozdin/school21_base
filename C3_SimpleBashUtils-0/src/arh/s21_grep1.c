#define _GNU_SOURSE  // getline

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *pattern;
  size_t size;
  int regeg_flag;  // -e шаблон, -i CaMelCaSe, "none flags"
  bool v;  // выводит строки которые не содержат grep -v "e" 123.txt
  bool c;  // выводит количество совпадений
  bool l;  // выводит имя файла где есть совпадение
  bool n;  // нумерует строки
  // bool h;  // выводит строки в множестве файлов без названия фалов
  // bool s;  // не выводит grep: 1243.txt: No such file or directory
  // bool f;  // file  получает регулярные выражения из файла
  bool o;  // печатает только совпадающие символы
} Flags;

char *xrealloc(char *block, size_t size) {
  char *temp;
  temp = realloc(block, size);
  if (!temp) {
    exit(errno);
  }
  return temp;
}

char *xmalloc(size_t size) {
  char *temp;
  temp = malloc(size);
  if (!temp) {
    exit(errno);
  }
  return temp;
}

size_t string_append_expr(char *string, size_t size, char const *expr,
                          size_t size_expr) {
  string = xrealloc(string, size + size_expr + 7);
  string[size] = '\\';
  string[size + 1] = '|';
  string[size + 2] = '\\';
  string[size + 3] = '(';
  memcpy(string + size, expr, size_expr);
  size += size_expr + 4;
  string[size] = '\\';
  string[size + 1] = ')';
  string[size + 2] = '\0';
  size += 3;
  return size;
}

Flags grep_read_flags(int argc, char *argv[]) {
  Flags flags = {NULL, 0, 0, false, false, false, false, false};
  int current_flag = getopt_long(argc, argv, "e:ivclno", 0, 0);

  flags.pattern = xmalloc(2);
  flags.pattern[0] = '\0';
  flags.pattern[1] = '\0';
  size_t pattern_size = 0;

  for (; current_flag != -1;
       current_flag = getopt_long(argc, argv, "e:ivclno", 0, 0)) {
    switch (current_flag) {
      case 'e':
        // temp_size = strlen(optarg);
        pattern_size = string_append_expr(flags.pattern, pattern_size, optarg,
                                          strlen(optarg));
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
      case 'o':
        flags.o = true;
        break;
    }
  }
  if (pattern_size) {
    flags.size = pattern_size;
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
  // printf("\n");
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

void grep_not_file() {
  char symbol;
  while (scanf("%c", &symbol)) {
    printf("%c", symbol);
  }
}

void grep_file(FILE *file, char *filename, Flags flags, regex_t *preg,
               int count_file) {
  (void)flags;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;

  int line_number = 0;

  // if (count_file == 0) {
  //   grep_not_file();
  // }

  while (getline(&line, &length, file) > 0) {
    line_number++;
    if (flags.v) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (flags.o)
          ;
        else {
          if (flags.n) {
            printf("%s:%i:%s", filename, line_number, line);
          } else {
            printf("%s", line);
          }
        }
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (flags.o) {
          if (flags.n) {
            printf("%s:%i:%.*s\n", filename, line_number,
                   (int)(match.rm_eo - match.rm_so), line + match.rm_so);
          } else {
            if (count_file == 1) {
              printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
            } else {
              printf("%s:%.*s\n", filename, (int)(match.rm_eo - match.rm_so),
                     line + match.rm_so);
            }
          }
          char *remaining = line + match.rm_eo;

          while (!regexec(preg, remaining, 1, &match, 0)) {
            if (flags.n) {
              printf("%s:%i:%.*s\n", filename, line_number,
                     (int)(match.rm_eo - match.rm_so), remaining + match.rm_so);
            } else {
              if (count_file == 1) {
                printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
              } else {
                printf("%s:%.*s\n", filename, (int)(match.rm_eo - match.rm_so),
                       remaining + match.rm_so);
              }
            }
            remaining = remaining + match.rm_eo;
          }
        } else {
          if (flags.n) {
            printf("%s:%i:%s", filename, line_number, line);
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
  }
  free(line);
}

void grep(int argc, char *argv[], Flags flags) {
  char **end = &argv[argc];
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  int count_file = 0;

  printf("%s", flags.pattern);

  if (flags.size == 0) {
    if (regcomp(preg, argv[0], flags.regeg_flag)) {
      fprintf(stderr, "failed to comple regex\n");
      exit(1);
    }
  } else {
    if (regcomp(preg, flags.pattern + 2, flags.regeg_flag)) {
      fprintf(stderr, "failed to comple regex\n");
      exit(1);
    }
  }
  free(flags.pattern);

  for (char **filename = argv + 1; filename != end; ++filename) {
    ++count_file;
    if (count_file >= 2) {
      break;
    }

    if (argc == 1) {
      if (flags.c) {
        grep_count(stdout, "", flags, preg, 1);
        // grep_not_file();
      } else {
        grep_file(stdout, "", flags, preg, 0);
        // grep_not_file();
      }
    }

    for (char **filename = argv + 1; filename != end; ++filename) {
      FILE *file = fopen(*filename, "rb");
      if (errno) {
        fprintf(stderr, "%s", argv[0]);
        perror(*filename);
        continue;
      }
      if (flags.l) {
        grep_file_name(file, *filename, flags, preg, count_file);
      }
      if (flags.c) {
        grep_count(file, *filename, flags, preg, argc);
      } else {
        grep_file(file, *filename, flags, preg, count_file);
      }
      fclose(file);
    }
  }
}

int main(int argc, char *argv[]) {
  Flags flags = grep_read_flags(argc, argv);
  argv += optind;
  argc -= optind;

  if (argc == 0) {
    printf("%s\t", "[--null] [pattern] [file ...]");
    exit(1);
  }
  grep(argc, argv, flags);

  return 0;
}
