#include <iostream>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <sys/types.h>
#include <semaphore.h>


int main()
{
    int filedes[2];

    pipe(filedes);
    pid_t pid;
    sem_t* semaphore = sem_open("/semaphore", O_CREAT, O_RDWR | S_IRWXU, 0);


    switch(pid = fork())
    { 
        case 0:
        {
            char message[80] = {};
            while(true)
            {
                read(filedes[0], message, 80);
                std::cout << "client: " << message << std::endl;
                for(size_t i = 0; i < 80; i++)
                    message[i] = '\0';
                sem_post(semaphore);
            }
            break;
        }
        case -1:
        {
            std::cout << "error" << std::endl;
            return 1;
        }
       
        default:
        {
            std::string message;
            while(true)
            {
                std::cout << "server: " ;
                do
                {
                    getline(std::cin, message);
                } while (message.empty());
                if(message == "return" && message.length() == 6)
                {
                    sem_unlink("/semaphore");
                    kill(getpid(), 0);
                    exit(0);
                }
                else
                {
                    write(filedes[1], message.c_str(), 80);
                    sem_wait(semaphore);
                }
                
            }
            break;
        }
    }

    return 0;
}