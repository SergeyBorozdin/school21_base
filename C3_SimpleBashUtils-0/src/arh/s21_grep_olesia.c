#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// S21_GREP FLAGS

bool ignor_upper = false;
bool invert_match = false;
bool count_match = false;
bool output_files = false;
bool e_flag = false;
bool number_lines = false;
bool no_filenames = false;
bool no_err = false;
bool f_flag = false;
bool multiple_files = false;
bool output_match = false;
bool first_iter = true;

// S21_GREP_FUNCS

void use_grep(char* filename, char* pattern);
void create_f_pattern(char* f_filename, char* array);
int compile_regex(char* line, char* pattern, int* search_status);
void print_match(char* filename, char* line, char* pattern, int line_num);
int check_for_files(int argc, int optind, char* argv[]);
void print_output(char* filename, char* line, int line_num);
void file_flags_output(char* filename, int match_num, int inv_match_num);
void print_usage();
void add_i_to_arr(char* optarg, char* array);

// GLOBALS

int file_num = 0;
int current_file = 0;

// MAIN

int main(int argc, char* argv[]) {
  opterr = 0;
  int value = 0;
  char* filename;
  char pattern[1024];
  char tmp_pattern[1024];
  int status_flag = 0;
  const char* short_options = "e:f:vclinhso";
  while ((value = getopt_long(argc, argv, short_options, 0, NULL)) != -1) {
    switch (value) {
      case 'e':
        e_flag = true;
        add_i_to_arr(optarg, pattern);
        break;
      case 'f':
        f_flag = true;
        create_f_pattern(optarg, tmp_pattern);
        add_i_to_arr(tmp_pattern, pattern);
        break;
      case 'i':
        ignor_upper = true;
        break;
      case 'v':
        invert_match = true;
        break;
      case 'c':
        count_match = true;
        break;
      case 'n':
        number_lines = true;
        break;
      case 'h':
        no_filenames = true;
        break;
      case 'l':
        output_files = true;
        break;
      case 's':
        no_err = true;
        break;
      case 'o':
        output_match = true;
        break;
      case '?':
        fprintf(stderr, "s21_grep: unknown option - %c. Try --help\n", optopt);
        status_flag = 1;
        break;
    }
  }
  if (optind == 1) {
    print_usage();
  } else if (status_flag != 1) {
    if (!e_flag) {
      if (!f_flag) {
        strcpy(pattern, argv[optind]);
        pattern[strlen(pattern)] = '\0';
        optind++;
      }
    }
    file_num = check_for_files(argc, optind, argv);
    if (file_num > 1) multiple_files = true;
    FILE* file;
    while ((optind < argc) && (status_flag != 1)) {
      filename = argv[optind];
      if ((file = fopen(filename, "r")) != NULL) {
        current_file++;
        fclose(file);
        use_grep(filename, pattern);
      } else {
        if (!no_err)
          fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                  filename);
        if (f_flag) status_flag = 1;
      }
      optind++;
    }
  }
  return status_flag;
}

void print_usage() {
  fputs("Usage: grep [OPTION]... PATTERNS [FILE]...\n", stdout);
}

void use_grep(char* filename, char* pattern) {
  FILE* file;
  int match_num = 0;
  int inv_match_num = 0;
  int line_num = 1;
  char line[4096];
  if (no_filenames) multiple_files = false;
  if ((file = fopen(filename, "r")) != NULL) {
    while (fgets(line, sizeof(line), file)) {
      int search_status;
      int compiled_value = compile_regex(line, pattern, &search_status);
      if (compiled_value == 0) {
        if ((search_status == 0) && (!invert_match)) {
          match_num++;
          if ((!count_match) && (!output_files) && (!output_match)) {
            print_output(filename, line, line_num);
          } else if (output_match) {
            if ((!count_match) && (!output_files))
              print_match(filename, line, pattern, line_num);
          }
        } else if ((invert_match) && (search_status == 1)) {
          inv_match_num++;
          if ((!count_match) && (!output_files) && (!output_match)) {
            print_output(filename, line, line_num);
          }
        }
      }
      line_num++;
    }
    file_flags_output(filename, match_num, inv_match_num);
    if ((current_file < file_num - 1) && (!output_files) && (!count_match) &&
        (!output_match))
      printf("%c", '\n');
  } else {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
  }
  fclose(file);
}

// Deal with files_output flags

void file_flags_output(char* filename, int match_num, int inv_match_num) {
  if (((count_match) || (output_files)) & (!invert_match)) {
    if (((count_match) && (output_files)) || (output_files)) {
      if (match_num > 0) {
        printf("%s", filename);
        printf("%c", '\n');
      }
    } else {
      if ((multiple_files) && (!no_filenames)) printf("%s", filename);
      if ((!multiple_files) && (output_files)) printf("%s", filename);
      if ((count_match) && (no_filenames)) {
        printf("%d\n", match_num);
      } else if (count_match && multiple_files) {
        printf(":%d\n", match_num);
      } else
        printf("%d\n", match_num);
    }
  }
  if ((invert_match) && (count_match)) {
    if ((multiple_files) || (output_files)) {
      if (output_files)
        printf("%s\n", filename);
      else {
        printf("%s:", filename);
        printf("%d", inv_match_num);
        printf("%c", '\n');
      }
    } else {
      printf("%d", inv_match_num);
      printf("%c", '\n');
    }
  } else if ((invert_match) && (output_files)) {
    printf("%s", filename);
    printf("%c", '\n');
  }
}

// Last step output

void print_output(char* filename, char* line, int line_num) {
  if ((multiple_files) && (!no_filenames)) printf("%s:", filename);
  if (number_lines) {
    printf("%d:", line_num);
  }
  printf("%s", line);
}

// Compile regex and extract matching indexes

int compile_regex(char* line, char* pattern, int* search_status) {
  regex_t regex;
  regmatch_t match;
  int result;
  if (ignor_upper)
    result = regcomp(&regex, pattern, REG_ICASE | REG_EXTENDED);
  else
    result = regcomp(&regex, pattern, REG_EXTENDED);
  if (result == 0) {
    *search_status = regexec(&regex, line, 1, &match, 0);
  }
  regfree(&regex);
  return result;
}

// Print only matching words

void print_match(char* filename, char* line, char* pattern, int line_num) {
  regex_t regex;
  regmatch_t pmatch[1];
  char* ptr = line;
  int search_status;
  regoff_t len = 1;
  if (ignor_upper)
    regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
  else
    regcomp(&regex, pattern, REG_EXTENDED);
  while ((search_status = regexec(&regex, ptr, 1, pmatch, 0)) == 0) {
    len = pmatch[0].rm_eo - pmatch[0].rm_so;
    if ((multiple_files) && (!no_filenames)) printf("%s:", filename);
    if (number_lines) {
      printf("%d:", line_num);
    }
    // printf("%d", len);
    printf("%.*s\n", (int)len, ptr + pmatch[0].rm_so);
    ptr += pmatch[0].rm_eo;
  }
  regfree(&regex);
}

// Function to create the pattern for f_flag

void create_f_pattern(char* f_filename, char* array) {
  char* ptr = array;
  FILE* file;
  if ((file = fopen(f_filename, "r")) != NULL) {
    char ch;
    int rep_nl = 0;
    while ((!feof(file) && !ferror(file))) {
      int tmp = getc(file);
      if (tmp != EOF) ch = tmp;
      if (ch == '\n') rep_nl = 1;
      if (rep_nl == 1) {
        *ptr = '|';
        ptr++;
        rep_nl = 0;
      } else {
        *ptr = ch;
        ptr++;
      }
    }
    ptr--;
    ptr--;
    *ptr = '\0';
    fclose(file);
  } else {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", f_filename);
  }
}

// create an array of filenames for -f and args for -e

void add_i_to_arr(char* optarg, char* array) {
  char* from_ptr = optarg;
  char* to_ptr = array;
  int offset;
  if (!first_iter) {
    offset = strlen(array);
  } else {
    offset = 0;
  }
  to_ptr += offset;
  if (offset != 0) {
    *to_ptr = '|';
    to_ptr++;
    offset++;
  }
  while (*from_ptr) {
    offset++;
    *to_ptr = *from_ptr;
    to_ptr++;
    from_ptr++;
  }
  array[offset] = '\0';
  first_iter = false;
}

// Check if char* argv[] hase more than one file

int check_for_files(int argc, int optind, char* argv[]) {
  int count_files = 0;
  while (optind < argc) {
    char* filename;
    FILE* file;
    filename = argv[optind];
    if ((file = fopen(filename, "r")) != NULL) {
      count_files++;
      fclose(file);
    }
    optind++;
  }
  return count_files;
}
