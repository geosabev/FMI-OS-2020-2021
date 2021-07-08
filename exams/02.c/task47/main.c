#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        errx(1, "Three arguments needed!");
    }

    struct triple
    {
        uint16_t first;
        uint8_t second;
        uint8_t third;
    } t;

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1)
    {
        err(2, "Open failed!");
    }
    
    struct stat st1;
    if (fstat(fd1, &st1) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(3, "Stat failed!");
    }

    if (st1.st_size % sizeof(t) != 0)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(4, "Wrong format!");
    }

    int fd2;
    if ((fd2 = open(argv[2], O_RDONLY)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(2, "Open failed!");
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

    if (st2.st_size % sizeof(uint8_t) != 0)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(4, "Wrong format!");
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

    //copy
    int r_status1;
    uint8_t number1;
    while ((r_status1 = read(fd2, &number1, sizeof(number1))) == sizeof(number1))
    {
        int w_status;
        if ((w_status = write(fd3, &number1, sizeof(number1))) != sizeof(number1))
        {
            int errno_cp = errno;
            close(fd1);
            close(fd2);
            close(fd3);
            errno = errno_cp;
            err(6, "Write failed!");
        }
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

    close(fd2);

    if (lseek(fd3, 0, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd3);
        errno = errno_cp;
        err(7, "lseek failed!");
    }

    //change
    int r_status2;
    uint8_t number2;
    while ((r_status2 = read(fd1, &t, sizeof(t))) == sizeof(t))
    {
        if (lseek(fd3, t.first, SEEK_SET) == -1)
        {
            int errno_cp = errno;
            close(fd1);
            close(fd3);
            errno = errno_cp;
            err(7, "lseek failed!");
        }

        int r_status3;
        if ((r_status3 = read(fd3, &number2, sizeof(number2))) != sizeof(number2))
        {     
            int errno_cp = errno;
            close(fd1);
            close(fd3);
            errno = errno_cp;
            err(5, "Read failed!");
        }

        if (number2 == t.second)
        {
            if (lseek(fd3, -1, SEEK_CUR) == -1)
            {
                int errno_cp = errno;
                close(fd1);
                close(fd3);
                errno = errno_cp;
                err(7, "lseek failed!");
            }

            int w_status;
            if ((w_status = write(fd3, &t.third, sizeof(t.third))) != sizeof(t.third))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd3);
                errno = errno_cp;
                err(6, "Write failed!");
            }
        }
        else
        {
            int errno_cp = errno;
            close(fd1);
            close(fd3);
            errno = errno_cp;
            err(8, "Invalid data!");
        }
    }

    if (r_status2 == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd3);
        errno = errno_cp;
        err(5, "Read failed!");
    }

    close(fd1);
    close(fd3);

    exit(0);
}
