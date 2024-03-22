#include "stdlib.h"
#include <stdio.h>

int main(int argc, char **argv, char **envp) {
	printf("argv: ");
	for (int i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	putchar('\n');

	FILE *proc_cmdline = fopen("/proc/self/cmdline", "r");
	/*
	if (!proc_cmdline) {
		perror("fopen");
		abort();
	}
	if (fseek(proc_cmdline, 0, SEEK_END)) {
		perror("fseek");
		abort();
	}
	size_t cmdline_size = ftell(proc_cmdline);
	fprintf(stderr, "cmdline_size: %zu\n", cmdline_size);
	if (cmdline_size == -1) {
		perror("ftell");
		abort();
	}
	if (fseek(proc_cmdline, 0, SEEK_SET)) {
		perror("fseek");
		abort();
	}
	char cmdline[cmdline_size];
	if (fread(cmdline, 1, cmdline_size, proc_cmdline) != cmdline_size) {
		perror("fread");
		abort();
	}
	*/

	printf("/proc/self/cmdline: ");

	char c;
	while (fread(&c, 1, 1, proc_cmdline) == 1) {
		if (c == '\0') {
			putchar(' ');
		} else {
			putchar(c);
		}
	}
	if (!feof(proc_cmdline)) {
		perror("fread");
		abort();
	}
}
