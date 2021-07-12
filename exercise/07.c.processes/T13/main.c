#include <stdlib.h>
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
    if (argc != 3)
    {
        errx(1, "Two arguments needed!");
    }

    int id1 = fork();

    if (id1 == -1)
    {
        err(2, "First fork failed!");
    }

    if (id1 == 0)
    {
        if (execlp(argv[1], argv[1], (char*)NULL) == -1)
        {
            err(3, "First command failed to execute!");
        }
    }

    int id2 = fork();

    if (id2 == -1)
    {
        err(4, "Second fork failed!");
    }

    if (id2 == 0)
    {
        if (execlp(argv[2], argv[2], (char*)NULL) == -1)
        {
            err(5, "Second command failed to execute!");
        }
    }

    int status, first;

    first = waitpid(-1, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        printf("PID of first finished process: %d\n", first);
    }
    else
    {
        err(6, "Fail!");
    }

    exit(0);
}
