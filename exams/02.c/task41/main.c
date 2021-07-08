#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//man 3 qsort
int cmp (const void* a, const void* b)
{
	uint8_t x = *((uint8_t*)a);
	uint8_t y = *((uint8_t*)b);

	if (x > y)
    {
		return -1;
	}	
	else if (x < y)
	{
		return 1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "One argument needed!");
    }

    //man 2 stat
    struct stat st;

    if (stat(argv[1], &st) == -1)
	{
		err(2, "Stat failed!");
	}

    if (st.st_size == 0)
    {
        errx(3, "Input file empty!");
    }

    int fd;
    if ((fd = open(argv[1], O_RDWR)) == -1)
    {
        err(4, "Open failed!");
    }

    uint8_t* arr = (uint8_t*)malloc(st.st_size);

    int r_status;
    if ((r_status = read(fd, arr, st.st_size)) != st.st_size)
    {
        int errno_cp = errno;
        close(fd);
        free(arr);
        errno = errno_cp;
        err(5, "Read failed!");
    }

    qsort(arr, st.st_size, sizeof(uint8_t), cmp);

    close(fd);

    if ((fd = open(argv[1], O_TRUNC | O_WRONLY)) == -1)
    {
        int errno_cp = errno;
        free(arr);
        errno = errno_cp;
        err(6, "Second open failed!");
    }
    
    int w_status;
    if ((w_status = write(fd, arr, st.st_size)) != st.st_size)
    {
        int errno_cp = errno;
        close(fd);
        free(arr);
        errno = errno_cp;
        err(7, "Overwrite failed!");
    }

    close(fd);
    free(arr);
    
    exit(0);
}
