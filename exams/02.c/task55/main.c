#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    //six files
    if (argc != 7)
    {
        errx(1, "Six arguments needed!");
    }

    //open all files
    int fd1;
    if ((fd1 = open(argv[6], O_RDONLY)))
    {
        err(2, "Open failed!");
    }

    int fd2;
    if ((fd2 = open(argv[6], O_RDONLY)))
    {
        int errno_cp = errno;
        close(fd1);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    int fd3;
    if ((fd3 = open(argv[6], O_RDONLY)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    int fd4;
    if ((fd4 = open(argv[6], O_RDONLY)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    int fd5;
    if ((fd5 = open(argv[6], O_RDONLY)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    int fd6;
    if ((fd6 = open(argv[6], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    //counters
    uint16_t affix_c, postfix_c, prefix_c, infix_c, suffix_c;

    //move to counter bytes (5 and 6)
    if (lseek(fd1, 4, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd2, 4, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd3, 4, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd4, 4, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd5, 4, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    //read all counter bytes
    int r_status;
    if ((r_status = read(fd1, &affix_c, sizeof(uint16_t))) != sizeof(uint16_t))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd2, &postfix_c, sizeof(uint16_t))) != sizeof(uint16_t))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd3, &prefix_c, sizeof(uint16_t))) != sizeof(uint16_t))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd4, &infix_c, sizeof(uint16_t))) != sizeof(uint16_t))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd5, &suffix_c, sizeof(uint16_t))) != sizeof(uint16_t))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    //move to first number in each file (17th byte)
    if (lseek(fd1, 16, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd2, 16, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd3, 16, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd4, 16, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    if (lseek(fd5, 16, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(3, "lseek failed!");
    }

    //allocate dymanic arrays
    uint16_t* affix_arr = (uint16_t*)malloc(affix_c * sizeof(uint16_t));
    uint32_t* postfix_arr = (uint32_t*)malloc(postfix_c * sizeof(uint32_t));
    uint8_t* prefix_arr = (uint8_t*)malloc(prefix_c * sizeof(uint8_t));
    uint16_t* infix_arr = (uint16_t*)malloc(infix_c * sizeof(uint16_t));
    uint64_t* suffix_arr = (uint64_t*)malloc(suffix_c * sizeof(uint64_t));

    //read all numbers from files
    if ((r_status = read(fd1, affix_arr, affix_c * sizeof(uint16_t))) != (ssize_t)(affix_c * sizeof(uint16_t)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd2, postfix_arr, postfix_c * sizeof(uint32_t))) != (ssize_t)(postfix_c * sizeof(uint32_t)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd3, prefix_arr, prefix_c * sizeof(uint8_t))) != (ssize_t)(prefix_c * sizeof(uint8_t)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd4, infix_arr, infix_c * sizeof(uint16_t))) != (ssize_t)(infix_c * sizeof(uint16_t)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    if ((r_status = read(fd4, suffix_arr, suffix_c * sizeof(uint64_t))) != (ssize_t)(suffix_c * sizeof(uint64_t)))
    {
        int errno_cp = errno;
        close(fd1);
        close(fd2);
        close(fd3);
        close(fd4);
        close(fd5);
        close(fd6);
        errno = errno_cp;
        err(4, "read failed!");
    }

    for (size_t y = 0; y < affix_c;)
    {
        printf(" affix x %x ;t %x\n", affix_arr[y], affix_arr[y + 1]);

        uint16_t x_post, t_post, x_pre, t_pre, x_in, t_in, x_suf, t_suf;
        
        x_post = affix_arr[y++];
        t_post = affix_arr[y++];
        
        printf("postfix array:\n");
        
        for (size_t pos = x_post, i = 0; i < t_post; i++)
        {
            printf("%x ", postfix_arr[pos]);

            uint32_t buf = postfix_arr[pos++];
            
            if ((write(fd6, &buf, sizeof(uint32_t))) != sizeof(uint32_t))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                close(fd4);
                close(fd5);
                close(fd6);
                errno = errno_cp;
                err(5, "write failed!");
            }
        }

        printf("\n");

        printf(" affix x %x ;t %x\n", affix_arr[y], affix_arr[y + 1]);
        
        x_pre = affix_arr[y++];
        t_pre = affix_arr[y++];
        
        printf("prefix array:\n");
        
        for (size_t pos = x_pre, i = 0; i < t_pre; i++)
        {
            printf("%x ", prefix_arr[pos]);

            uint8_t buf = prefix_arr[pos++];

            if ((write(fd6, &buf, sizeof(uint8_t))) != sizeof(uint8_t))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                close(fd4);
                close(fd5);
                close(fd6);
                errno = errno_cp;
                err(5, "write failed!");
            }
        }

        printf("\n");

        printf(" аffix x %x ;t %x\n", affix_arr[y], affix_arr[y + 1]);

        x_in = affix_arr[y++];
        t_in = affix_arr[y++];

        printf("infix array:\n");

        for (size_t pos = x_in, i = 0; i < t_in; i++)
        {
            printf("%x ", infix_arr[pos]);

            uint16_t buf = infix_arr[pos++];

            if ((write(fd6, &buf, sizeof(uint16_t))) != sizeof(uint16_t))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                close(fd4);
                close(fd5);
                close(fd6);
                errno = errno_cp;
                err(5, "write failed!");
            }
        }

        printf("\n");

        printf(" affix x %x ;t %x\n", affix_arr[y], affix_arr[y + 1]);

        x_suf = affix_arr[y++];
        t_suf = affix_arr[y++];

        printf("suffix array:\n");

        for (size_t pos = x_suf, i = 0; i < t_suf; i++)
        {
            printf("%lx ", suffix_arr[pos]);

            uint64_t buf = suffix_arr[pos++];

            if ((write(fd6, &buf, sizeof(uint64_t))) != sizeof(uint64_t))
            {
                int errno_cp = errno;
                close(fd1);
                close(fd2);
                close(fd3);
                close(fd4);
                close(fd5);
                close(fd6);
                errno = errno_cp;
                err(5, "write failed!");
            }
        }

        printf("\n");
    }

    free(affix_arr);
    free(postfix_arr);
    free(prefix_arr);
    free(infix_arr);
    free(suffix_arr);

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);
    close(fd6);

    exit(0);
}

