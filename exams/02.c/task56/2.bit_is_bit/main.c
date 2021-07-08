#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        errx(1, "Invalid arguments!");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1)
    {
        err(2, "Open of .scl file failed!");
    }

    struct stat st1;
    if (fstat(fd1, &st1) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(3, "Stat of .scl file failed!");
    }

    if (st1.st_size % sizeof(uint8_t) != 0)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(4, "Invalid format of .scl file!");
    }

    int fd2;
    if ((fd2 = open(argv[2], O_RDONLY)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(2, "Open of .sdl file failed!");
    }

    struct stat st2;
    if (fstat(fd2, &st2) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(3, "Stat of .sdl file failed!");
    }

    if (st2.st_size % sizeof(uint16_t) != 0 && (ssize_t)(st2.st_size / sizeof(uint16_t)) != (st1.st_size * 8))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(4, "Invalid format of .sdl file!");
    }

    int fd;
    if ((fd = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(2, "Creation of output .sdl file failed!");
    }

    uint8_t byte;
    uint16_t n;

    int r_status1, r_status2;
    while ((r_status1 = read(fd1, &byte, sizeof(byte))) == sizeof(byte))
    {
        for (int i = 1; i <= 8; i++)
        {
            if ((r_status2 = read(fd2, &n, sizeof(n))) != sizeof(n))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd);
                errno = errno_cp;
                err(5, "Reading of numbers failed!");
            }

            if (byte & (1 << (i - 1)))
            {
                if ((write(fd, &n, sizeof(n))) != sizeof(n))
                {
                    int errno_cp = errno;
                    close(fd1);
                    close(fd2);
                    close(fd);
                    errno = errno_cp;
                    err(6, "Write failed!");
                }
            }
        }
    }

    if (r_status1 == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd);
        errno = errno_cp;
        err(5, "Reading of levels failed!");
    }

    close(fd1);
    close(fd2);
    close(fd);

    exit(0);
}
