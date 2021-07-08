#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int cmp(const void* a, const void* b)
{
    return *((uint32_t*)a) - *((uint32_t*)b);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "One argument needed!");
    }

    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        err(2, "Open failed!");
    }

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(5, "Stat failed!");
    }

    if (st.st_size == 0)
    {
        exit(0);
    }

    if (st.st_size % sizeof(uint32_t) != 0)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(6, "Invalid format!");
    }

    uint32_t half_count = ((st.st_size / sizeof(uint32_t)) / 2);
	uint32_t* half1 = (uint32_t*)malloc(half_count * sizeof(uint32_t));

    int r_status;
    if ((r_status = read(fd, half1, half_count * sizeof(uint32_t))) != (ssize_t)(sizeof(uint32_t) * half_count))
    {
        int errno_cp = errno;
        close(fd);
        free(half1);
        errno = errno_cp;
        err(3, "Read failed!");
    }

    qsort(half1, half_count, sizeof(uint32_t), cmp);

    int fd_t1;
    if ((fd_t1 = open("temp1", O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(fd);
        free(half1);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    int w_status;
    if ((w_status = write(fd_t1, half1, half_count * sizeof(uint32_t))) != (ssize_t)(half_count * sizeof(uint32_t)))
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        free(half1);
        errno = errno_cp;
        err(4, "Write failed!");
    }

    free(half1);

    if (lseek(fd_t1, 0, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        errno = errno_cp;
        err(7, "Lseek failed!");
    }

    uint32_t* half2 = (uint32_t*)malloc(half_count * sizeof(uint32_t));
    
    if ((r_status = read(fd, half2, half_count * sizeof(uint32_t))) != (ssize_t)(half_count * sizeof(uint32_t)))
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        free(half2);
        errno = errno_cp;
        err(3, "Read failed!");
    }

    qsort(half2, half_count, sizeof(uint32_t), cmp);

    int fd_t2;
    if ((fd_t2 = open("temp2", O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        free(half2);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    if ((w_status = write(fd_t2, half2, half_count * sizeof(uint32_t))) != (ssize_t)(half_count * sizeof(uint32_t)))
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        close(fd_t2);
        free(half2);
        errno = errno_cp;
        err(4, "Write failed!");
    }

    free(half2);

    if (lseek(fd_t2, 0, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        close(fd_t2);
        errno = errno_cp;
        err(6, "Lseek failed!");
    }

    close(fd);

    if ((fd = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(fd_t1);
        close(fd_t2);
        errno = errno_cp;
        err(2, "Open failed");
    }

    uint32_t i = 0, j = 0;
	uint32_t c1, c2;

    int r_status2;
	while (((r_status = read(fd_t1, &c1, sizeof(c1))) == sizeof(c1)) && ((r_status2 = read(fd_t2, &c2, sizeof(c2))) == sizeof(c2)))
    {
		if (c1 <= c2)
        {
			if ((write(fd, &c1, sizeof(c1))) == -1)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_t1);
                close(fd_t2);
                errno = errno_cp;
                err(4, "Write failed!");
            }
            
            if (lseek(fd_t2, -1 * sizeof(c2), SEEK_CUR) == -1)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_t1);
                close(fd_t2);
                errno = errno_cp;
                err(6, "Lseek failed!");
            }
			
            i++;
		}
		else
        {
            if ((write(fd, &c2, sizeof(c2))) == -1)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_t1);
                close(fd_t2);
                errno = errno_cp;
                err(4, "Write failed!");
            }

            if (lseek(fd_t1, -1 * sizeof(c1), SEEK_CUR) == -1)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_t1);
                close(fd_t2);
                errno = errno_cp;
                err(6, "Lseek failed!");
            }

            j++;
		}
	}

    if (r_status == -1)
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        close(fd_t2);
        errno = errno_cp;
        err(3, "Read failed!");
    }

    if (r_status2 == -1)
    {
        int errno_cp = errno;
        close(fd);
        close(fd_t1);
        close(fd_t2);
        errno = errno_cp;
        err(3, "Read failed!");
    }

	while (i < half_count && (read(fd_t1, &c1, sizeof(c1))) == sizeof(c1))
    {
	    if ((write(fd, &c1, sizeof(c1))) != sizeof(c1))
        {
            int errno_cp = errno;
            close(fd);
            close(fd_t1);
            close(fd_t2);
            errno = errno_cp;
            err(4, "Write failed!");
        }

		i++;
	}

    while (j < half_count && (read(fd_t2, &c2, sizeof(c2))) == sizeof(c2))
    {
        if ((write(fd, &c2, sizeof(c2))) != sizeof(c2))
        {
            int errno_cp = errno;
            close(fd);
            close(fd_t1);
            close(fd_t2);
            errno = errno_cp;
            err(4, "Write failed!");
        }

        j++;
    }

	close(fd);
	close(fd_t1);
	close(fd_t2);

    if ((remove("temp1")) == -1)
    {
        err(7, "Remove failed!");
    }

    if ((remove("temp2")) == -1)
    {
        err(7, "Remove failed!");
    }

    exit(0);
}
