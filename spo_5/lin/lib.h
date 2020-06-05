#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <aio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

extern int8_t toggle;
extern int count;
extern struct aiocb cb;
extern char buffer[256];

void* thread_write(void* args);
void* thread_read(void* args);

