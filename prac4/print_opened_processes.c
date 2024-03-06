#include <stdio.h>
#include <windows.h>

#include <tlhelp32.h>

void try_open_and_print(PROCESSENTRY32 *process_entry) {
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_entry->th32ProcessID);
	if (process) {
		printf("successfully opened %s\n", process_entry->szExeFile);
		if (!CloseHandle(process)) {
			fprintf(stderr, "CloseHandle: error %ld\n", GetLastError());
			abort();
		}
	} else {
		printf("failed to open %s\n", process_entry->szExeFile);
	}
}

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

	try_open_and_print(&process_entry);

	while (Process32Next(snapshot, &process_entry)) {
		try_open_and_print(&process_entry);
	}

	if (GetLastError() != ERROR_NO_MORE_FILES) {
		fprintf(stderr, "Process32Next: error %ld\n", GetLastError());
		abort();
	}

	if (!CloseHandle(snapshot)) {
		fprintf(stderr, "CloseHandle: error %ld\n", GetLastError());
		abort();
	}

	return 0;
}
