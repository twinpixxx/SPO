#include <pthread.h>
#include "lib.h"

int main()
{
  pthread_t writer, reader;

  pthread_create(&writer, NULL, &thread_write, NULL);
  pthread_create(&reader, NULL, &thread_read, NULL);

  pthread_join(writer, NULL);
  pthread_join(reader, NULL);
  
  return 0;
}

