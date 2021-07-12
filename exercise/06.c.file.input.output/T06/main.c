#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        errx(1, "At least one argument needed!");
    }

    int fd, r_stat, w_stat;
    char c;

    for (int i = 1; i < argc; i++)
    {
        if ((fd = open(argv[i], O_RDONLY)) == -1)
        {
            err(2, "Open failed!");
        }

        while ((r_stat = read(fd, &c, 1)) != -1 && r_stat != 0)
        {
            if ((w_stat = write(1, &c, 1)) == -1)
            {
                int errno_cp = errno;
                close(fd);
                errno = errno_cp;
                err(3, "Write failed!");
            }
        }

        close(fd);
    }

    exit(0);
}
