#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct options {
  int bflag;
  int nflag;
  int eflag;
  int sflag;
  int tflag;
  int vflag;
} opt;

struct option opts[] = {
    {"squeeze-blank", no_argument, 0, 's'},
    {"number-nonblank", no_argument, 0, 'b'},
    {"E", no_argument, 0, 'e'},
    {"number", no_argument, 0, 'n'},
    {"T", no_argument, 0, 't'},
    {"v", no_argument, 0, 'v'},
    {0, 0, 0, 0},
};

void ifflag(int *cur, int *prev, int *i, int *empt, opt options);
void flag_turn(int argc, char **argv, opt *options);
void mainwork(int argc, char **argv, int *currfile, opt options);