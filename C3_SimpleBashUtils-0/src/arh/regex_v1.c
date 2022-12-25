#include <locale.h>
#include <regex.h>
#include <stdio.h>

#define BUFSIZE 256

int match(char *string, char *pattern, regex_t *re) {
  int status;

  if ((status = regcomp(re, pattern, REG_EXTENDED)) != 0) return (status);
  status = regexec(re, string, 0, NULL, 0);
  return (status);
}

int main() {
  char *p;
  char *pattern[] = {"hello[0-9]*", "1234"};
  char *string = "this is a test string hello112 and this is test";
  /* Строка для сравнения */

  int retval;
  regex_t re;
  char buf[BUFSIZE];
  int i;

  setlocale(LC_ALL, "");

  for (i = 0; i < 2; i++) {
    retval = match(string, pattern[i], &re);
    if (retval == 0) {
      printf("Обнаружено соответствие \n");
    } else {
      regerror(retval, &re, buf, BUFSIZE);
      printf("ошибка = %s\n", buf);
    }
  }
  regfree(&re);
}
