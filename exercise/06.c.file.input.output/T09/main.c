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
    if(argc != 3)
    {
        errx(1, "Two arguments needed!");
    }

    int option1 = strcmp(argv[1], "--print");
    int option2 = strcmp(argv[1], "--min");
    int option3 = strcmp(argv[1], "--max");

    if (option1 != 0 && option2 != 0 && option3 != 0)
    {
        errx(2, "No such opition supported!");
    }

    uint16_t buffer, min = UINT16_MAX, max = 0;
    int fd, r_stat;
    
    if ((fd = open(argv[2], O_RDONLY)) == -1)
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

        if (min > buffer)
        {
            min = buffer;
        }

        if (option1 == 0)
        {
            printf("%d\n", buffer);
        }
    }

    if (option2 == 0)
    {
        printf("Min: %d\n", min);
    }
    else if (option3 == 0)
    {
        printf("Max: %d\n", max);
    }

    close(fd);

    exit(0);
}
