#include "pch.h"
#include <iostream>
#include <Windows.h>

bool read(const char* path, const char* buffer, unsigned int from, unsigned int to)
{
	OVERLAPPED structure;
	structure.Offset = NULL;
	structure.OffsetHigh = NULL;
	structure.hEvent = NULL;
	std::cout << "read" << std::endl;
	HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	ReadFile(file, (LPVOID)buffer, BUFSIZ, NULL, &structure);
	std::cout << "..." << std::endl;
	WaitForSingleObject(file, INFINITE);
	CloseHandle(file);
	return true;
}

bool write(const char* path, const char* buffer, unsigned int from, unsigned int howMany)
{
	OVERLAPPED structure;
	structure.Offset = from;
	structure.OffsetHigh = NULL;
	structure.hEvent = NULL;
	std::cout << "write" << std::endl;
	HANDLE file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	WriteFile(file, buffer, howMany, NULL, &structure);
	std::cout << "..." << std::endl;
	WaitForSingleObject(file, INFINITE);
	CloseHandle(file);
	return true;
}