#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        errx(1, "One argument needed!");
    }

    uint8_t buffer, max = 0;
    int fd, r_stat;
    
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        err(3, "Open failed!");
    }

    while ((r_stat = read(fd, &buffer, sizeof(buffer))) != 0)
    {
        if (r_stat == -1)
        {
            int errno_cpy = errno;
            close(fd);
            errno = errno_cpy;
            err(4, "Read failed!");
        }

        if (buffer > max)
        {
            max = buffer;
        }
    }

    printf("max byte: %d\n", max);

    close(fd);

    exit(0);
}
