#include <stdio.h>
#include <windows.h>

#include <tlhelp32.h>

#include <psapi.h>

void try_print_full_path(PROCESSENTRY32 *process_entry) {
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_entry->th32ProcessID);
	if (!process) {
		printf("failed to open %s\n", process_entry->szExeFile);
		abort();
	}

	CHAR path[MAX_PATH];
	if (!GetModuleFileNameExA(process, NULL, path, sizeof(path) / sizeof(*path))) {
		fprintf(stderr, "GetModuleFileNameExA: error %ld\n", GetLastError());
		abort();
	}

	printf("%s: %s\n", process_entry->szExeFile, path);

	if (!CloseHandle(process)) {
		fprintf(stderr, "CloseHandle: error %ld\n", GetLastError());
		abort();
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

	try_print_full_path(&process_entry);

	while (Process32Next(snapshot, &process_entry)) {
		try_print_full_path(&process_entry);
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
