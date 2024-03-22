#include "fileapi.h"
#include "handleapi.h"
#include "memoryapi.h"
#include "stdlib.h"
#include "winnt.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define PRINT_WINAPI_ERROR(x) fprintf(stderr, "%s: error %ld\n", x, GetLastError())

int main() {
	HANDLE file = CreateFileA("example_file.txt", GENERIC_ALL, 0, NULL, OPEN_EXISTING,
				  FILE_ATTRIBUTE_NORMAL, NULL);
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

	MEMORY_BASIC_INFORMATION memory_info;
	if (!VirtualQuery(content, &memory_info, sizeof(memory_info))) {
		PRINT_WINAPI_ERROR("VirtualQuery");
		abort();
	}

	printf("%.*s", (int)memory_info.RegionSize, (char *)content);

	strcpy(content, "amogus");
	printf("%s\n", (char *)content);

	return 0;
}
