#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

int cmp(const void* a, const void*b )
{
    return *((uint16_t*)a) - *((uint16_t*)b);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "Two arguments needed!");
    }

    int fd1;
    if ((fd1 = open(argv[1], O_RDONLY)) == -1)
    {
        err(2, "Open failed!");
    }

    struct stat st;
    if (fstat(fd1, &st) == -1)
    {
        err(3, "Stat failed!");
    }

    if (st.st_size % sizeof(uint16_t) != 0)
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(4, "Invalid format!");
    }

    ssize_t count = st.st_size / sizeof(uint16_t);
    uint16_t* buffer = malloc(count * sizeof(uint16_t));

    uint16_t r_status;
    if ((r_status = read(fd1, buffer, sizeof(uint16_t) * count)) != (count * sizeof(uint16_t)))
    {
        int errno_cp = errno;
        close(fd1);
        free(buffer);
        errno = errno_cp;
        err(5, "Read failed!");
    }

    close(fd1);

    qsort(buffer, count, sizeof(uint16_t), cmp);

    int fd2;
    if ((fd2 = open(argv[2], O_CREAT | O_TRUNC| O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        free(buffer);
        errno = errno_cp;
        err(2, "Open failed!");
    }
    
    if ((r_status = write(fd2, buffer, sizeof(uint16_t) * count)) != (count * sizeof(uint16_t)))
    {
        int errno_cp = errno;
        close(fd2);
        free(buffer);
        errno = errno_cp;
        err(6, "Write failed!");
    }

    close(fd2);
    free(buffer);

    exit(0);
}
