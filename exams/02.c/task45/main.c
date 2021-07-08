#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        errx(1, "Three arguments needed!");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1)
    {
        err(2, "Open failed!");
    }

    int fd2;
    if ((fd2 = open(argv[2], O_RDONLY)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    struct stat st1;
    if (fstat(fd1, &st1) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(3, "Stat failed!");
    }

    struct stat st2;
    if (fstat(fd2, &st2) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(3, "Stat failed!");
    }

    if (st1.st_size != st2.st_size)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(4, "Different file format!");
    }

    int fd3;
    if ((fd3 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    struct triple
    {
        uint16_t first;
        uint8_t second;
        uint8_t third;
    };

    int r_status1, r_status2;
    uint8_t n1, n2;
    uint16_t pos = 0x00;
    struct triple t;

    while ((r_status1 = read(fd1, &n1, sizeof(n1))) > 0 && (r_status2 = read(fd2, &n2, sizeof(n2))) > 0)
    {
        if (n1 != n2)
        {
            t.first = pos;
            t.second = n1;
            t.third = n2;

            int w_status;
            if ((w_status = write(fd3, &t, sizeof(t))) != sizeof(t))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                errno = errno_cp;
                err(6, "Write failed!");
            }
        }

        pos++;
    }

    if (r_status1 == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        errno = errno_cp;
        err(5, "Read failed!");
    }

    if (r_status2 == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        errno = errno_cp;
        err(5, "Read failed!");
    }

    close(fd1);
    close(fd2);
    close(fd3);

    exit(0);
}
