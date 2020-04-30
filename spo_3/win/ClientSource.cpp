#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>
#include <windows.h>
#include <string>


HANDLE fileProjectionDescriptor;
LPVOID fileAddr;

int main()
{
	char message[80] = {};

	fileProjectionDescriptor = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, "allocated-memory");
	fileAddr = MapViewOfFile(fileProjectionDescriptor, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);

	HANDLE semaphore = OpenSemaphoreA(EVENT_ALL_ACCESS, FALSE, "semaphore");
	ReleaseSemaphore(semaphore, 1, 0);


	while (true)
	{
		//char* message = (char*)malloc(80 * sizeof(char));
		WaitForSingleObject(semaphore, INFINITE);
		CopyMemory(message, fileAddr, 80);

		std::cout << message << std::endl;
		for (size_t i = 0; i < 80; i++)
			message[i] = '\0';
		//delete(message);
		//message = NULL;
		ReleaseSemaphore(semaphore, 1, NULL);
	}
	return 0;
}