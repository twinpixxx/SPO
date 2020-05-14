#include<Windows.h>
#include<iostream>
#include <conio.h>

#define MAX_QTY_OF_THREADS 15

using namespace std;

CRITICAL_SECTION criticalSection;
size_t id = 0;

DWORD WINAPI print(LPVOID LParam) {
	while (id < MAX_QTY_OF_THREADS) {
		EnterCriticalSection(&criticalSection);
		cout << "Work thread " << id << ": " << GetCurrentThreadId() << endl;
		LeaveCriticalSection(&criticalSection);
		WaitForSingleObject(GetCurrentThread(), 1500);
	}
	ExitThread(0);
}

int main() {
	InitializeCriticalSection(&criticalSection);
	char userChoice;
	HANDLE threads[MAX_QTY_OF_THREADS];
	cout << "+ - Create Thread" << endl
		 << "- - Kill Thread " << endl
		 << "x - Exit" << endl;
	while (true) {
		userChoice = _getch();
		if (userChoice == 'x') {
			DeleteCriticalSection(&criticalSection);
			for (size_t i = 0; i < id; i++) {
				TerminateThread(threads[id - 1], 0);
			}
			return 1;
		} else if (userChoice == '+') {
			if (id < MAX_QTY_OF_THREADS) {
				threads[id] = CreateThread(nullptr,
					0,
					print,
					nullptr,
					CREATE_SUSPENDED,
					nullptr);
				cout << "start thread  " << GetThreadId(threads[id]) << endl;
				ResumeThread(threads[id++]);
			}
		} else if (userChoice == '-') {
			if (id > 0) {
				cout << "kill thread " << GetThreadId(threads[--id]) << endl;
				TerminateThread(threads[id], 0);
			} else {
				cout << "No threads" << endl;
			}
		}
	}
	return 0;
}