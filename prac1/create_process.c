#include <stdio.h>
#include <windows.h>

int main() {
	LPPROCESS_INFORMATION process_information = malloc(sizeof(*process_information));
	LPSTARTUPINFOA startup_info = malloc(sizeof(*startup_info));

	ZeroMemory(process_information, sizeof(*process_information));

	if (!CreateProcessA("C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, 0, 0, NULL,
			    NULL, startup_info, process_information)) {
		printf("CreateProcessA: error %ld\n", GetLastError());
		return 1;
	}

	Sleep(1000);

	if (!TerminateProcess(process_information->hProcess, 1)) {
		printf("TerminateProcess: error %ld\n", GetLastError());
		return 1;
	}

	return 0;
}
