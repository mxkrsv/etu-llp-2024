#include <stdio.h>

int main(int argc, char **argv, char **envp) {
	for (size_t i = 0; envp[i] != NULL; i++) {
		printf("%s\n", envp[i]);
	}
}
