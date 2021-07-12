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
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        errx(1, "Three arguments needed!");
    }

    int fd, id1, id2;

    if ((fd = open(argv[3], O_CREAT|O_TRUNC|O_RDWR, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH)) == -1)
    {
        err(2, "Open failed!");
    }

    id1 = fork();

    if (id1 == -1)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(3, "First fork failed!");
    }
    
    if (id1 == 0)
    {
        if (execlp(argv[1], argv[1], (char*)NULL) == -1)
        {
            err(4, "Could not execlp first command!");
        }
    }

    int status1;

    wait(&status1);

    if (WIFEXITED(status1))
    {
        if (WEXITSTATUS(status1) == 0)
        {
            int w_status1;
            ssize_t len1 = (ssize_t)strlen(argv[1]);

            if ((w_status1 = write(fd, argv[1], len1)) != len1)
            {
                int errno_cp = errno;
                close(fd);
                errno = errno_cp;
                err(5, "First write failed!");
            }

            if ((w_status1 = write(fd, "\n", 1)) != 1)
            {
                int errno_cp = errno;
                close(fd);
                errno = errno_cp;
                err(5, "First write failed!");
            }
        }
    }

    id2 = fork();

    if (id2 == -1)
    {
        err(6, "Second fork() failed!");
    }

    if (id2 == 0)
    {
        if (execlp(argv[2], argv[2], (char*)NULL) == -1)
        {
            err(7, "Second command failed to execute!");
        }
    }

    int status2;

    wait(&status2);

    if (WIFEXITED(status2))
    {
        if (WEXITSTATUS(status2) == 0)
        {
            int w_status2;
            ssize_t len2 = (ssize_t)strlen(argv[2]);

            if ((w_status2 = write(fd, argv[2], len2)) != len2)
            {
                int errno_cp = errno;
                close(fd);
                errno = errno_cp;
                err(8, "Second write failed!");
            }

            if ((w_status2 = write(fd, "\n", 1)) != 1)
            {
                int errno_cp = errno;
                close(fd);
                errno = errno_cp;
                err(8, "Second write failed!");
            }
        }
    }

    close(fd);

    exit(0);
}
