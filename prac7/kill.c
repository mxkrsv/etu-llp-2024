#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "exatly one argument is required: PID\n");
		return 1;
	}

	if (kill(atoi(argv[1]), SIGKILL)) {
		perror("kill");
		abort();
	}
}
