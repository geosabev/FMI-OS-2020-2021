#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "One argument (name of copy) needed!");
    }

    int fd, fd_new, r_stat, w_stat;
    char c;

    if ((fd=open("/etc/passwd", O_RDONLY)) == -1)
    {
        err(2, "Opening target file failed!");
    }

    if ((fd_new=open(argv[1], O_CREAT | O_RDWR | O_TRUNC, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(3, "Opening copy file failed!");
    }

    while ((r_stat = read(fd, &c, 1)) != -1 && r_stat != 0)
    {
        if (c == ':')
        {
            c = '?';
        }

        if ((w_stat = write(fd_new, &c, 1)) != 1)
        {
            int errno_cp = errno;
            close(fd);
            close(fd_new);
            errno = errno_cp;
            err(4, "Writing failed!");
        }
    }
    
    close(fd);
    close(fd_new);

    exit(0);
}
