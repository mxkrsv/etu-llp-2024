#include <stdbool.h>
#include <stdio.h>
#include <synchapi.h>
#include <windows.h>

typedef struct thread_args {
	bool exit_flag;
	char *text_to_print;
	CRITICAL_SECTION *critical_section;
} thread_args;

DWORD WINAPI MyThreadFunc(LPVOID arg) {
	thread_args *args = (thread_args *)arg;

	while (!args->exit_flag) {
		EnterCriticalSection(args->critical_section);

		Sleep(1);
		printf("%s\n", args->text_to_print);

		LeaveCriticalSection(args->critical_section);
		Sleep(1);
	}

	return 0;
}

int main() {
	CRITICAL_SECTION critical_section;
	InitializeCriticalSection(&critical_section);

	thread_args thread1_args = {.exit_flag = false,
				    .critical_section = &critical_section,
				    .text_to_print = "Hello from thread 1!"};
	thread_args thread2_args = {.exit_flag = false,
				    .critical_section = &critical_section,
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
