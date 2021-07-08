#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>

int main()
{
    int fd1, fd2, fd3;
    struct stat st1;
    struct stat st2;
    
    struct pair32_t
    {
        uint32_t first;     //start
        uint32_t second;    //length
    } p;

    if (stat("f1", &st1) == -1)
    {
        err(1, "f1 stat failed!");
    }

    if (stat("f2", &st2) == -1)
    {
        err(1, "f2 stat failed!");
    }

    if (st1.st_size % sizeof(p) != 0)
    {
        err(2, "f1 format incorrect!");
    }

    if (st2.st_size % sizeof(uint32_t) != 0)
    {
        err(2, "f2 format incorrect!");
    }

    if ((fd1 = open("f1", O_RDONLY)) == -1)
    {
        err(3, "open failed!");
    }

    if ((fd2 = open("f2", O_RDONLY)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(3, "open failed!");
    }

    if ((fd3 = open("fd3", O_CREAT|O_TRUNC|O_WRONLY, S_IWUSR|S_IRUSR)) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(3, "open failed!");
    }

    int r_status1;

    while ((r_status1 = read(fd1, &p, sizeof(p))) > 0)
    {
        uint32_t st = p.first;
        uint32_t len = p.second;

        off_t ls = lseek(fd2, (st - 1) * sizeof(uint32_t), SEEK_SET);

        if (ls == -1)
        {
            int errno_cp = errno;
            close(fd1);
            close(fd2);
            close(fd3);
            errno = errno_cp;
            err(5, "lseek failed!");
        }
        
        uint32_t n, i = 0;
        int r_status2;
        
        while ((r_status2 = read(fd2, &n, sizeof(n))) > 0)
        {
            if (i == len)
            {
                break;
            }

            int w_status;

            if ((w_status = write(fd3, &n, sizeof(n)) != sizeof(n)))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                errno = errno_cp;
                err(6, "write failed!");
            }

            i++;
        }

        if (r_status2 == -1)
        {
            int errno_cp = errno;
            close(fd1);
            close(fd2);
            close(fd3);
            errno = errno_cp;
            err(4, "read failed!");
        }
    }

    if (r_status1 == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        errno = errno_cp;
        err(4, "read failed!");
    }

    close(fd1);
    close(fd2);
    close(fd3);
}
