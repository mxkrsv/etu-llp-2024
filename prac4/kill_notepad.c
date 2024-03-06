#include <stdio.h>
#include <windows.h>

#include <tlhelp32.h>

void try_kill_notepad(PROCESSENTRY32 *process_entry) {
	if (strcmp(process_entry->szExeFile, "notepad.exe")) {
		printf("not a notepad: %s\n", process_entry->szExeFile);
		return;
	}

	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_entry->th32ProcessID);
	if (!process) {
		printf("failed to open %s\n", process_entry->szExeFile);
		abort();
	}

	if (!TerminateProcess(process, 0)) {
		fprintf(stderr, "TerminateProcess: error %ld\n", GetLastError());
		abort();
	}

	printf("successfully killed %s\n", process_entry->szExeFile);

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

	try_kill_notepad(&process_entry);

	while (Process32Next(snapshot, &process_entry)) {
		try_kill_notepad(&process_entry);
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
