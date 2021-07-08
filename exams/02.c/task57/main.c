#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "Invalid arguments!");
    }

    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        err(2,"Test open failed!");
    }
    close(fd);

    int p[2];
    if (pipe(p) == -1)
    {
        err(3, "Pipe failed!");
    }

    int id = fork();
    if (id == -1)
    {
        err(4, "Fork failed!");
    }

    //child is reading from cat and writing to pipe
    if (id == 0)
    {
        close(p[0]);

        if (dup2(p[1], 1) == -1)
        {
            err(5, "dup2 failed!"); 
        }

        close(p[1]);

        if (execl("/bin/cat", "cat", argv[1], (char*)NULL) == -1)
        {
            err(6, "Execution of cat failed!");
        }
    }

    //parent is reading from pipe and writing to sort
    close(p[1]);

    if (dup2(p[0], 0) == -1)
    {
        err(7, "dup2 failed!");
    }

    close(p[0]);

    if (execlp("sort", "sort", (char*)NULL) == -1)
    {
        err(8, "Execution of sort failed!");
    }

    exit(0);
}
