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
    if (argc != 2)
    {
        errx(1, "One argument needed!");
    }

    int fd;

    if ((fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        err(2, "Open failed!");
    }

    int id = fork();

    if (id == 0)
    {
        char* s = "foo";
        int w_status;

        if ((w_status = write(fd, s, 3)) != 3)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(4, "Write failed!");
        }

        exit(0);
    }
    else if (id < 0)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(3, "Fork failed!");
    }

    int status;

    wait(&status);

    id = fork();

    if (id == 0)
    {
        char* s = "bar";
        int w_status;

        if ((w_status = write(fd, s, 3)) != 3)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(4, "Write failed!");
        }

        exit(0);
    }

    close(fd);

    exit(0);
}
