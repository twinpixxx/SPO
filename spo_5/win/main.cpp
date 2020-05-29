// main.cpp

#include <stdio.h>
#include <Windows.h>

#define TEXT_FILE_QUANTITY 5

typedef bool (*foo)(const char*, const char*, unsigned int, unsigned int);	

HMODULE dll;	
CRITICAL_SECTION writeSec;
CRITICAL_SECTION delSec;

DWORD WINAPI threadWrite(LPVOID ptr)
{
	char path[] = "../store/result.txt";
	int pos = 0;
	int quantity = 0;

	foo writeF = (foo)GetProcAddress(dll, "write");

	while (!TryEnterCriticalSection(&delSec))
	{
		EnterCriticalSection(&writeSec);
		for (int i = 0; i != BUFSIZ && ((char*)ptr)[i] != '\0'; i++)
			quantity++;
		writeF(path, (const char*)ptr, pos, quantity);
		pos += quantity;
		quantity = 0;
		LeaveCriticalSection(&writeSec);
		Sleep(70);
	}
	LeaveCriticalSection(&delSec);
	return 0;
}

int main()
{
	char path[] = "../store/0.txt";
	unsigned int count = 1;
	char buffer[BUFSIZ] = {};

	dll = LoadLibraryA("../../lab5dll/Debug/lab5dll.dll");
	foo readF = (foo)GetProcAddress(dll, "read");

	InitializeCriticalSection(&writeSec);			
	InitializeCriticalSection(&delSec);

	EnterCriticalSection(&writeSec);
	EnterCriticalSection(&delSec);

	CreateThread(NULL, NULL, threadWrite, buffer, NULL, NULL);

	bool flagFirst = true;

	for (; count <= TEXT_FILE_QUANTITY; count++)
	{
		if (flagFirst)
		{
			path[9]--;
			count--;
			flagFirst = false;
		}
		path[9]++;
		readF(path, buffer, 0, 0);
		LeaveCriticalSection(&writeSec);
		Sleep(50);
		EnterCriticalSection(&writeSec);
		if (count + 1 > TEXT_FILE_QUANTITY)
			LeaveCriticalSection(&delSec);
		for (int i = 0; i != BUFSIZ && buffer[i] != '\0'; i++)
			buffer[i] = {};
		Sleep(100);
	}

	EnterCriticalSection(&delSec);

	DeleteCriticalSection(&writeSec);
	DeleteCriticalSection(&delSec);

	FreeLibrary(dll);

	return 0;
}