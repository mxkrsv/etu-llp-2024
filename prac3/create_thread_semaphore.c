#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <synchapi.h>
#include <windows.h>

typedef struct thread_args {
	bool exit_flag;
	char *text_to_print;
	HANDLE semaphore;
} thread_args;

DWORD WINAPI MyThreadFunc(LPVOID arg) {
	thread_args *args = (thread_args *)arg;

	while (!args->exit_flag) {
		WaitForSingleObject(args->semaphore, INFINITE);

		Sleep(100);
		printf("%s\n", args->text_to_print);

		ReleaseSemaphore(args->semaphore, 1, NULL);
	}

	return 0;
}

int main() {
	HANDLE semaphore = CreateSemaphoreA(NULL, 1, LONG_MAX, NULL);
	if (!semaphore) {
		fprintf(stderr, "CreateMutexA: error %lu\n", GetLastError());
		abort();
	}

	thread_args thread1_args = {.exit_flag = false,
				    .semaphore = semaphore,
				    .text_to_print = "Hello from thread 1!"};
	thread_args thread2_args = {.exit_flag = false,
				    .semaphore = semaphore,
				    .text_to_print = "Hello from thread 2!"};

	HANDLE thread_handles[2];

	if (!(thread_handles[0] = CreateThread(NULL, 0, MyThreadFunc, &thread1_args, 0, NULL))) {
		fprintf(stderr, "CreateThread: error %lu\n", GetLastError());
		abort();
	}

	if (!(thread_handles[1] = CreateThread(NULL, 0, MyThreadFunc, &thread2_args, 0, NULL))) {
		fprintf(stderr, "CreateThread: error %lu\n", GetLastError());
		abort();
	}

	printf("Press Enter\n");

	getchar();

	thread1_args.exit_flag = true;
	thread2_args.exit_flag = true;

	if (WaitForMultipleObjects(2, thread_handles, TRUE, INFINITE) == WAIT_FAILED) {
		fprintf(stderr, "WaitForMultipleObjects: error %lu\n", GetLastError());
		abort();
	}

	printf("waited\n");

	return 0;
}
