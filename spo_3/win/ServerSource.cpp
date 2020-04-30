#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>
#include <windows.h>
#include <string>


HANDLE fileProjectionDescriptor;
LPVOID fileAddr;

int main()
{
	std::string message;

	fileProjectionDescriptor = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 80, "allocated-memory");
	fileAddr = MapViewOfFile(fileProjectionDescriptor, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);


	STARTUPINFOA  StartUpInformation;
	ZeroMemory(&StartUpInformation, sizeof(STARTUPINFOA));
	PROCESS_INFORMATION ProcessInfo;
	CreateProcessA("./Client.exe", NULL, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &StartUpInformation, &ProcessInfo);

	HANDLE semaphore = CreateSemaphoreA(NULL, 0, 1, "semaphore");
	WaitForSingleObject(semaphore, INFINITE);


	while (true)
	{
		std::cin >> message;
		CopyMemory(fileAddr, message.c_str(), 80);
		ReleaseSemaphore(semaphore, 1, 0);
		WaitForSingleObject(semaphore, INFINITE);
		if (message == "return" && message.length() == 6)
		{
			TerminateProcess(ProcessInfo.hProcess, 0);
			UnmapViewOfFile(fileAddr);
			CloseHandle(semaphore);
			CloseHandle(fileProjectionDescriptor);
			break;
		}
	}

	return 0;
}