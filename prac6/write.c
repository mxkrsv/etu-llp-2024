#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
	int fd = open("test_file", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("open");
		abort();
	}

	char s[32];
	scanf("%31s", s);

	if (write(fd, s, strlen(s)) != strlen(s)) {
		perror("write");
		abort();
	}

	if (close(fd)) {
		perror("close");
		abort();
	}

	fd = open("test_file", O_RDONLY);
	if (fd == -1) {
		perror("open");
		abort();
	}

	char r[32];
	if (read(fd, r, strlen(s)) != strlen(s)) {
		perror("read");
		abort();
	}

	printf("read from file: %.*s\n", (int)strlen(s), r);

	if (lseek(fd, strlen(s) / 2, SEEK_SET) == -1) {
		perror("lseek");
		abort();
	}

	if (read(fd, r, strlen(s) - strlen(s) / 2) != strlen(s) - strlen(s) / 2) {
		perror("read");
		abort();
	}

	printf("read from the middle of the file: %.*s\n", (int)(strlen(s) - strlen(s) / 2), r);

	if (close(fd)) {
		perror("close");
		abort();
	}

	if (unlink("test_file")) {
		perror("unlink");
		abort();
	}

	return 0;
}
