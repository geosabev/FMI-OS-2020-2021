#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "One argument needed!");
    }

    int p_fd[2];

    if (pipe(p_fd) == -1)
    {
        err(2, "Piping failed!");
    }

    int pid = fork();

    if (pid == -1)
    {
        err(3, "Fork failed!");
    }

    if (pid == 0)
    {
        close(p_fd[1]);
        
        dup2(p_fd[0], 0);

        if (execlp("wc", "wc", "-c", (char*)NULL) == -1)
        {
            err(4, "Could not execute wc -c!");
        }
    }

    close(p_fd[0]);
    
    if (write(p_fd[1], argv[1], strlen(argv[1])) != (int)strlen(argv[1]))
    {
        err(5, "Writing failed!");
    }

    close(p_fd[1]);

    wait(NULL);

    exit(0);
}
