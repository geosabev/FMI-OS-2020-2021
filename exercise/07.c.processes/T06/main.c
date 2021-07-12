#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "One argument needed!");
    }
    
    int id = fork();
    int status;

    if (id > 0)
    {
        //father process
        wait(&status);

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 0)
            {
                printf("%s\n", argv[1]);
            }
        }    
    }
    else if (id == 0)
    {
        // child process
        if (execlp(argv[1], argv[1], (char *)NULL) == -1)
        {
            err(1, "Execution failed!");
        }
    }
    else
    {
        err(2, "Fork failed!");
    }

    exit(0);
}
