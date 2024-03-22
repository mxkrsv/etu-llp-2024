#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int fd = open("test_file", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("open");
		abort();
	}

	return 0;
}
