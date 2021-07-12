#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        err(1, "One argument needed!");
    }

    int fd;

    if ((fd = open(argv[1], O_CREAT|O_TRUNC|O_RDWR, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH)) == -1)
    {
        err(2, "Open failed!");
    }

    int w_status1;

    if ((w_status1 = write(fd, "fo", 2)) != 2)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(3, "Write failed!");
    }

    int id = fork();

    if (id == -1)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(4, "Fork failed!");
    }

    if (id == 0)
    {
        int w_status2;

        if ((w_status2 = write(fd, "bar\n", 4)) != 4)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(3, "Write failed!");
        }

        close(fd);

        exit(0);
    }

    int status;

    wait(&status);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        int w_status3;

        if ((w_status3 = write(fd, "o\n", 2)) != 2)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(3, "Write failed!");
        }
    }
    else
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(5, "Child had problem!");
    }

    close(fd);

    exit(0);
}
