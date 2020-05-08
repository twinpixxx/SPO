#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>  
#include <unistd.h>    
#include <string.h>  
#include <stdlib.h>

#include <stdio.h>
#include <iostream>
using namespace std;

#define SHMSIZE 20
#define UNIQENUMBER 2009
void client(char*, int);

int main(int argc, char* argv[]) {

	char* shm;
	key_t key;
	int pid;
	int choice;
	key = shmget(UNIQENUMBER, SHMSIZE, 0666 | IPC_CREAT);
	if ((shm = (char*)shmat(key, (void*)0, 0)) == (char*)(-1)) {
		printf("Can't attach shared memory\n");
		exit(-1); 
	}
	char* s = shm;
	while (true) {
		fflush(stdin);
		string str;
		printf("Enter string [PID: %d]: ", getpid());
		getline(cin, str); 
		strcpy(s, str.c_str());
		pid = fork();
		switch (pid) {
		    case 0: {
		        client(shm, key);
		        printf("Exit? 1/0\n");
		        do {
		            fflush(stdin);
		            cin >> choice;
		        } while (choice < 0 || choice > 1);
		        if (choice == 1) {
		            kill(pid, SIGTERM);
		            exit(0);
		        }
		        kill(getpid(), SIGTERM);
		        break;
		    } case -1: { 
		        perror("ERROR");
    		    exit(0);
		    } default: {
		        wait(NULL); 

		        break;
		    }
	    }
	}
	return 0;
}

void client(char* sharedMemory, int key) {
	key = shmget(UNIQENUMBER, SHMSIZE, 0);       
	sharedMemory = (char*)shmat(key, (void*)0, 0);
	printf("Child wrote [PID: %d][PPID: %d]: %s\n", getpid(), getppid(), sharedMemory);  
	shmdt(sharedMemory);            
}

