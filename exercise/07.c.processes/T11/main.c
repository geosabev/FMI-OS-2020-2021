#include<stdlib.h>
#include<stdio.h>
#include<err.h>
#include<errno.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        errx(1, "At least one argument needed!");
    }

    int id, status, success = 0, failed = 0;

    for (int i = 1; i < argc; i++)
    {
        id = fork();

        if (id == -1)
        {
            err(2, "Process failed to fork()!");
        }
        else if (id == 0)
        {
           if (execlp(argv[i], argv[i], (char*)NULL) == -1)
           {
                err(3, "Failed to execute command!");
           }
        }

        wait(&status);

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 0)
            {
                success++;
            }
            else
            {
                failed++;
            }
        }
    }
    
    printf("Successful commands: %d\nFailed commands: %d\n", success, failed);

    exit(0);
}
