#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd = open("t.txt", O_WRONLY);
	for (int i = 0; i != 34; ++i) {
		printf("\"%c\", ", i);
	}
}

char const *print_unprintable(int c) {
	char const *code[32] = {"^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G",
	"^H", " ", "\v", "^K", "^L", "^M", "^N", "^O", "^P", "^Q", "^R", "^S",
	"^T", "^U", "^V", "^W", "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  
	return (code[c]);
}
