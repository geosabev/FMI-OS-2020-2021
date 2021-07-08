#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        errx(1, "Four arguments needed!");
    }

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

    if (st1.st_size % sizeof(uint8_t) != 0)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(4, "Invalid format!");
    }

    struct triplet
    {
        uint16_t offset;
        uint8_t length;
        uint8_t reserved;
    } tr;

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

    if (st2.st_size % sizeof(tr) != 0)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(4, "Invalid format!");
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

    int fd4;
    if ((fd4 = open(argv[4], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        errno = errno_cp;
        err(2, "Open failed!");
    }
    
    int r_status1;
    uint8_t start, holder;
    uint16_t pos = 0;
    struct triplet add;
    while ((r_status1 = read(fd2, &tr, sizeof(tr))) == sizeof(tr))
    {        
        if (lseek(fd1, tr.offset, SEEK_SET) == -1)
        {
            int errno_cp = errno;
            close(fd1);
            close(fd2);
            close(fd3);
            close(fd4);
            errno = errno_cp;
            err(6, "lseek failed!");
        }

        if ((read(fd1, &start, sizeof(start))) != sizeof(start))
        {
            int errno_cp = errno;
            close(fd1);
            close(fd2);
            close(fd3);
            close(fd4);
            errno = errno_cp;
            err(5, "Read failed!");
        }
    
        if (start >= 0x41 && start <= 0x5A)
        {
            if (lseek(fd1, -1, SEEK_CUR) == -1)
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                close(fd4);
                errno = errno_cp;
                err(6, "lseek failed!");
            }

            for (int i = 1; i <= tr.length; i++)
            {
                if ((read(fd1, &holder, sizeof(holder))) != sizeof(holder))
                {
                    int errno_cp = errno;
                    close(fd1);
                    close(fd2);
                    close(fd3);
                    close(fd4);
                    errno = errno_cp;
                    err(5, "Read failed!");
                }

                if ((write(fd3, &holder, sizeof(holder))) != sizeof(holder))
                {
                    int errno_cp = errno;
                    close(fd1);
                    close(fd2);
                    close(fd3);
                    close(fd4);
                    errno = errno_cp;
                    err(7, "Write failed!");
                }
            }

            add.offset = pos;
            add.length = tr.length;
            add.reserved = tr.reserved;

            if ((write(fd4, &add, sizeof(add))) != sizeof(add))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                close(fd4);
                errno = errno_cp;
                err(7, "Write failed!");
            }

            pos += tr.length;
        }
    }

    if (r_status1 == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        errno = errno_cp;
        err(5, "Read failed!");
    }

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    exit(0);
}
