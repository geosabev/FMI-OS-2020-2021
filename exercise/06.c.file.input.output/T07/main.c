#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        errx(1, "At least two arguments needed!");
    }

    int fd, fd_target, r_stat;
    char buffer[4096];

    if ((fd_target = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH )) == -1)
    {
        err(2, "Open failed!");
    }

    for (int i = 1; i < argc - 1; i++)
    {
        if ((fd = open(argv[i], O_RDONLY)) == -1)
        {
            int errno_cp = errno;
            close(fd_target);
            errno = errno_cp;
            err(3, "Open failed!");
        }

        while ((r_stat = read(fd, &buffer, sizeof(buffer))) != -1 && r_stat != 0)
        {
            if (write(fd_target, &buffer, r_stat) != r_stat)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_target);
                errno = errno_cp;
                err(3, "Write failed!");
            }
        }

        close(fd);        
    }

    close(fd_target);

    exit(0);
}
