#include "handleapi.h"
#include <stdio.h>
#include <windows.h>

#include <tlhelp32.h>

int main() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "CreateToolhelp32Snapshot: error %ld\n", GetLastError());
		abort();
	}

	PROCESSENTRY32 process_entry = {.dwSize = sizeof(PROCESSENTRY32)};

	if (!Process32First(snapshot, &process_entry)) {
		fprintf(stderr, "Process32First: error %ld\n", GetLastError());
		abort();
	}

	printf("%s\n", process_entry.szExeFile);

	while (Process32Next(snapshot, &process_entry)) {
		printf("%s\n", process_entry.szExeFile);
	}

	if (GetLastError() != ERROR_NO_MORE_FILES) {
		fprintf(stderr, "Process32Next: error %ld\n", GetLastError());
		abort();
	}

	CloseHandle(snapshot);

	return 0;
}
