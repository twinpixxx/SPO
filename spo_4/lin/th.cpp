#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

using namespace std;

struct ThreadInfo {
	pthread_t thread;
};

struct MutexInfo {
	pthread_mutex_t mutex;
};

void *printID(void *arg);
void init(struct MutexInfo *);
struct ThreadInfo createNewThread(struct MutexInfo *);
void deleteOneThread(struct ThreadInfo);
void deleteAll(stack <struct ThreadInfo> &s);
int _getch();

int main() {
    stack<struct ThreadInfo> threads;

    struct MutexInfo mInfo;
    init(&mInfo);

    while (true) {
        switch (_getch()) {
            case '+': {
                threads.push(createNewThread(&mInfo));
                break;
            } case '-': {
                if (threads.size()) {
                    deleteOneThread(threads.top());
                    threads.pop();
                }
                break;
            } case 'q': {
                deleteAll(threads);
                return 0;
            }
        }
    }
    return 0;
}

int _getch() {
    struct termios Old, New;
    char ch;
    tcgetattr(0, &Old);
    New = Old;
    New.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &New);
    ch = getchar();
    tcsetattr(0, TCSANOW, &Old);
    return ch;
}

void* printID(void *arg) {
    usleep(1500000);
    while (true) {
        pthread_mutex_lock((pthread_mutex_t*)arg);
        printf("Thread id: %lu\n",pthread_self());
        pthread_mutex_unlock((pthread_mutex_t *)arg);
        usleep(1500000);
    }
}

void deleteOneThread(struct ThreadInfo info) {
    pthread_cancel(info.thread);
}

struct ThreadInfo createNewThread(struct MutexInfo *mInfo) {
    struct ThreadInfo info;
    pthread_create(&info.thread,
            NULL,
            printID,
            (void*)&mInfo->mutex);
    return info;
}

void init(struct MutexInfo *info) {
    pthread_mutex_init(&info->mutex, NULL);
}

void deleteAll(stack <struct ThreadInfo> &threads) {
    while(threads.size()) { 
        pthread_cancel(threads.top().thread);
        threads.pop();
    }
}