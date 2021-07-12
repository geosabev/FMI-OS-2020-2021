#include <errno.h>
#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "Two arguments needed!");
    }

    int fd1, fd2;

    if ((fd1=open(argv[1], O_RDONLY)) == -1)
    {
        err(2, "Open failed!");
    }

    if ((fd2=open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_reason = errno;
        close(fd1);
        errno = errno_reason;
        err(2, "Open failed!");
    }

    int s, w_stat;
    char buffer[4096];

    while ((s=read(fd1, &buffer, sizeof(buffer))) != -1 && s != 0)
    {
        if ((w_stat = write(fd2, &buffer, s)) != s)
        {
            int errno_reason = errno;
            close(fd1);
            close(fd2);
            errno = errno_reason;
            err(3, "Write failed!");
        }
    }

    close(fd1);
    close(fd2);

    exit(0);
}
