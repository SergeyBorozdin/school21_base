#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

int main (int argc, char **argv) {

    int is_bracets = 0;
    int opindex = 0;
    int opchar;
    char string[] = "Цитата известного русского поэта";
    int length = 0;

    struct option opts[] = {
        {"bracets", no_argument, &is_bracets, 1},
        {"length", required_argument, 0, 'l'},
        {"avtorname", no_argument, 0, 'a'},
        {0, 0, 0, 0},
    };

    while(-1 != (opchar = getopt_long(argc, argv, "bl:aq", opts, &opindex))) {
        switch(opchar) {
            case 0:
            printf("0\n");
            break;

            case 'l':
            length = atoi(optarg);
            printf("length=%d\n", length);
            break;

            case 'a':
            printf("(c) ПЕТР КОЗЛОВ\n");
            break;

            default:
            printf("optchar%c\n", opchar);
        }
    }

    if(strlen(string) > length && 0 != length) {
        string[length] ='\0';
    }
    if (is_bracets) {
        printf("[%s]\n", string);
    } else {
        printf("%s\n", string);
    }
    printf("Done\n");

}
