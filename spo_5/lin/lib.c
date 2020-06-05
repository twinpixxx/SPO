#include "lib.h"

int8_t toggle = FALSE;
int count = 5;
struct aiocb cb;
char buffer[256];


void* thread_write(void* args)
{
  int flags = O_CREAT | O_APPEND | O_WRONLY;

  int cat_file = open("concatenated", flags, 0644);
  
  if(-1 == cat_file)
    {
      perror("open");
      exit(EXIT_FAILURE);
    }

  while(count > 0)
    {
      while(TRUE != toggle)
	{
	  usleep(100);
	}

      cb.aio_nbytes = strlen(buffer);
      cb.aio_fildes = cat_file;
      
      if(-1 == aio_write(&cb))
	{
	  perror("aio_write");
	  close(cat_file);
	  exit(EXIT_FAILURE);
	}

      while(EINPROGRESS == aio_error(&cb))
	{

	}

      toggle = FALSE;
    }
  printf("Data has been written\n");
  pthread_exit(NULL);
}

void* thread_read(void *args)
{
  char file_name[256];

  memset(&cb, 0, sizeof(cb));
  cb.aio_offset = 0;
  cb.aio_buf = buffer;
  
  for (int i = 1; i < 6; i++)
    {
      while(TRUE == toggle)
	{
	  usleep(100);
	}

      sprintf(file_name, "%d", i);
      int current_file = open(file_name, O_RDONLY);

      if(-1 == current_file)
	{
	  perror("open");
	  exit(EXIT_FAILURE);
	}

      cb.aio_nbytes = 256;
      cb.aio_fildes = current_file;
      
      if(-1 == aio_read(&cb))
	{
	  perror("aio_read");
	  close(current_file);
	  exit(EXIT_FAILURE);
	}  

      while(aio_error(&cb) == EINPROGRESS)
	{
	  //
	}

      count--;
      toggle = TRUE;
      
      printf("%s", buffer);
      
      close(current_file);
    }
  printf("Data has been readed\n");
  
  pthread_exit(NULL);
}
