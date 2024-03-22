#include <windows.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <synchapi.h>

#define PRINT_WINAPI_ERROR(x) fprintf(stderr, "%s: error %ld\n", x, GetLastError())

typedef struct thread_args {
	bool exit_flag;
	char *text_to_print;
	void *ipc;
	HANDLE mutex;
} thread_args;

DWORD WINAPI MyThreadFunc(LPVOID arg) {
	thread_args *args = (thread_args *)arg;

	while (!args->exit_flag) {
		WaitForSingleObject(args->mutex, INFINITE);

		Sleep(100);
		strcpy(args->ipc, args->text_to_print);

		ReleaseMutex(args->mutex);
	}

	return 0;
}

int main() {
	HANDLE file = CreateFileA("ipc", GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
				  NULL);
	if (file == INVALID_HANDLE_VALUE) {
		PRINT_WINAPI_ERROR("CreateFileA");
		abort();
	}

	HANDLE mapping = CreateFileMappingA(file, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (!mapping) {
		PRINT_WINAPI_ERROR("CreateFileMappingA");
		abort();
	}

	void *content = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!content) {
		PRINT_WINAPI_ERROR("MapViewOfFile");
		abort();
	}

	/*
	MEMORY_BASIC_INFORMATION memory_info;
	if (!VirtualQuery(content, &memory_info, sizeof(memory_info))) {
		PRINT_WINAPI_ERROR("VirtualQuery");
		abort();
	}
	*/

	HANDLE mutex = CreateMutexA(NULL, FALSE, "fuck_windows");
	if (!mutex) {
		fprintf(stderr, "CreateMutexA: error %lu\n", GetLastError());
		abort();
	}

	thread_args thread1_args = {.exit_flag = false,
				    .mutex = mutex,
				    .text_to_print = "Hello from thread 1!",
				    .ipc = content};
	thread_args thread2_args = {.exit_flag = false,
				    .mutex = mutex,
				    .text_to_print = "Hello from thread 2!",
				    .ipc = content};

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
