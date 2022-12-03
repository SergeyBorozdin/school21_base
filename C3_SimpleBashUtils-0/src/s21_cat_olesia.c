#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void use_cat(char* filename);
void print_help();
unsigned char code_ascii(unsigned char ch, int* miss_char);

// S21_CAT FLAGS

bool number_nonempty = false;
bool end_of_line = false;
bool number_all = false;
bool squeeze = false;
bool rep_tabs = false;
bool show_hidden = false;

// LONG OPTIONS

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}};

// SHORT OPTIONS

const char* short_options = "benstvET";

// GLOBALS
int global_line_count = 1;

// MAIN

int main(int argc, char* argv[]) {
  opterr = 0;
  int value = 0;
  char* filename;
  int program_status = 0;
  while ((value = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    switch (value) {
      case 'b':
        number_nonempty = true;
        break;
      case 'e':
        end_of_line = true;
        show_hidden = true;
        break;
      case 'n':
        number_all = true;
        break;
      case 's':
        squeeze = true;
        break;
      case 't':
        rep_tabs = true;
        show_hidden = true;
        break;
      case 'v':
        show_hidden = true;
        break;
      case 'E':
        end_of_line = true;
        break;
      case 'T':
        rep_tabs = true;
        break;
      case 'h':
        print_help();
        program_status = 1;
        break;
      case '?':
        fprintf(stderr, "s21_cat: unknown option - %c. Try --help\n", optopt);
        program_status = 1;
        break;
    }
  }
  if (program_status != 1) {
    while ((optind < argc) && (program_status != 1)) {
      filename = argv[optind];
      FILE* file;
      if ((file = fopen(filename, "r")) != NULL) {
        fclose(file);
        use_cat(filename);
      } else {
        fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
        program_status = 1;
      }
      optind++;
    }
  }
  return 0;
}

void use_cat(char* filename) {
  FILE* file = fopen(filename, "r");
  unsigned char ch;
  char prev_char;
  int line_count = 1;
  int newline_count = 0;
  int miss_char = 0;
  int tmp;
  while ((!feof(file) && !ferror(file))) {
    tmp = getc(file);
    if (tmp != EOF) {
      ch = tmp;
      if ((squeeze) && (ch == '\n') && (prev_char == '\n')) {
        newline_count++;
        if (newline_count >= 2) {
          continue;
        }
      } else {
        newline_count = 0;
      }
      if (number_nonempty) {
        if ((line_count == 1) && (ch != '\n')) {
          printf("%6d\t", global_line_count);
          line_count++;
          global_line_count++;
        } else if ((prev_char == '\n') && (ch != '\n')) {
          printf("%6d\t", global_line_count);
          line_count++;
          global_line_count++;
        }
      }
      if ((number_all) && (!number_nonempty)) {
        if (line_count == 1) {
          printf("%6d\t", global_line_count);
          line_count++;
          global_line_count++;
        } else if (prev_char == '\n') {
          printf("%6d\t", global_line_count);
          line_count++;
          global_line_count++;
        }
      }
      if ((end_of_line) && (ch == '\n')) {
        printf("%c", '$');
      }
      if ((rep_tabs) && (ch == '\t')) {
        printf("%c", '^');
        printf("%c", 'I');
        miss_char = 1;
      }
      if (show_hidden == true) {
        ch = code_ascii(ch, &miss_char);
      }
      if (!miss_char) {
        printf("%c", ch);
      }
      miss_char = 0;
      prev_char = ch;
    }
  }
  fclose(file);
}

unsigned char code_ascii(unsigned char ch, int* miss_char) {
  if (ch >= 32) {
    if (ch < 127) {
      ch = ch;
    } else if (ch == 127) {
      printf("%c", '^');
      printf("%c", '?');
      *miss_char = 1;
    } else {
      printf("%c", 'M');
      printf("%c", '-');
      if (ch >= 128 + 32) {
        if (ch < 128 + 127)
          ch = ch - 128;
        else {
          printf("%c", '^');
          printf("%c", '?');
        }
      } else {
        printf("%c", '^');
        ch = ch - 128 + 64;
      }
    }
  } else if (ch == '\n')
    ch = ch;
  else if (ch == '\t')
    ch = ch;
  else {
    printf("%c", '^');
    ch = ch + 64;
  }
  return ch;
}

void print_help() {
  fputs(
      "\tCurrent S21_Cat flags:\n"
      "-b (GNU: --number-nonblank): numbers only non-empty lines\n"
      "-e implies -v (GNU only: -E the same, but without implying -v): but "
      "also display end-of-line characters as $\n"
      "-n (GNU: --number): number all output lines\n"
      "-s (GNU: --squeeze-blank): squeeze multiple adjacent blank lines\n"
      "-t implies -v (GNU: -T the same, but without implying -v): but also "
      "display tabs as ^I\n",
      stdout);
}
