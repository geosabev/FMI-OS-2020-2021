#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "Two arguments needed!");
    }

    int p[2];
    if (pipe(p) == -1)
    {
        err(2, "Piping failed!");
    }

    int id = fork();

    if (id == -1)
    {
        err(3, "Fork failed!");
    }

    if (id == 0)
    {
        close(p[0]);
        close(1);

        dup2(p[1], 1);

        if (execl("/bin/cat", "/bin/cat", argv[1], (char*)NULL) == -1)
        {
            err(4, "Execution of cat failed!");
        }
    }

    close(p[1]);

    int fd;
    if ((fd = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        err(5, "Open failed!");
    }

    char c;
    int r_status;
    while ((r_status = read(p[0], &c, sizeof(c))) == sizeof(c))
    {
        if (c == 0x7D)
        {
            if ((read(p[0], &c, sizeof(c))) != sizeof(c))
            {
                int errno_cp = errno;
                close(fd);
                close(p[0]);
                errno = errno_cp;
                err(7, "Invalid format!");
            }

            c = c ^ 0x20;
        }

        if ((write(fd, &c, sizeof(c)) != sizeof(c)))
        {
            int errno_cp = errno;
            close(fd);
            close(p[0]);
            errno = errno_cp;
            err(8, "Write failed!");
        }
    }

    if (r_status == -1)
    {
        int errno_cp = errno;
        close(fd);
        close(p[0]);
        errno = errno_cp;
        err(6, "Read failed!");
    }

    close(fd);
    close(p[0]);

    exit(0);
}
