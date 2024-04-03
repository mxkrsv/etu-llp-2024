#include <stdio.h>
#include <stdarg.h>

int example(int count, ...) {
	va_list args;

	va_start(args, count);

	int sum;
	while (count-- > 0) {
		sum += va_arg(args, int);
	}

	return sum;
}

int main() {
	printf("%d\n", example(5, 1, 2, 3, 4, 5));
}
