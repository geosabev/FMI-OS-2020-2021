#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    //count of arguments
    if (argc != 4)
    {
        errx(1, "Three arguments needed!");
    }

    //open patch
    int patch_fd;
    if ((patch_fd = open(argv[1], O_RDONLY)) == -1)
    {
        err(2, "Open failed!");
    }

    struct header
    {
        uint32_t magic;
        uint8_t header_version;
        uint8_t data_version;
        uint16_t count;
        uint32_t reserved1;
        uint32_t reserved2;
    } h;

    //read header
    int r_status;
    if ((r_status = read(patch_fd, &h, sizeof(h))) != sizeof(h))
    {
        int errno_cp = errno;
        close(patch_fd);
        errno = errno_cp;
        err(3, "Read failed!");
    }

    //validate header
    if (h.magic != 0xEFBEADDE || h.header_version != 0x01 || (h.data_version != 0x01 && h.data_version != 0x00))
    {
        int errno_cp = errno;
        close(patch_fd);
        errno = errno_cp;
        err(4, "Wrong format!");
    }

    //stat patch.bin
    struct stat st_patch;
    if (fstat(patch_fd, &st_patch) == -1)
    {
        int errno_cp = errno;
        close(patch_fd);
        errno = errno_cp;
        err(5, "Stat failed!");
    }

    //check size according to data_version
    if (h.data_version == 0x00)
    {
        if (st_patch.st_size != (ssize_t)((3 * sizeof(uint32_t)) + (2 * sizeof(uint8_t)) + sizeof(uint16_t) + (h.count * (sizeof(uint16_t) + (2 * sizeof(uint8_t))))))
        {
            int errno_cp = errno;
            close(patch_fd);
            errno = errno_cp;
            err(4, "Wrong format");
        }
    }
    else
    {
        if (st_patch.st_size != (ssize_t)((3 * sizeof(uint32_t)) + (2 * sizeof(uint8_t)) + sizeof(uint16_t) + (h.count * (sizeof(uint32_t) + (2 * sizeof(uint16_t))))))
        {
            int errno_cp = errno;
            close(patch_fd);
            errno = errno_cp;
            err(4, "Wrong format");
        }
    }

    //open f1
    int fd1;
    if ((fd1 = open(argv[2], O_RDONLY)) == -1)
    {
        int errno_cp = errno;
        close(patch_fd);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    //stat f1
    struct stat st1;
    if (fstat(fd1, &st1) == -1)
    {
        int errno_cp = errno;
        close(patch_fd);
        close(fd1);
        errno = errno_cp;
        err(5, "Stat failed!");
    }

    //check format
    if (h.data_version == 0x00)
    {
        if (st1.st_size % sizeof(uint8_t) != 0)
        {
            int errno_cp = errno;
            close(patch_fd);
            close(fd1);
            errno = errno_cp;
            err(4, "Wrong format!");
        }
    }
    else
    {
        if (st1.st_size % sizeof(uint16_t) != 0)
        {
            int errno_cp = errno;
            close(patch_fd);
            close(fd1);
            errno = errno_cp;
            err(4, "Wrong format!");
        }
    }

    //open f2
    int fd2;
    if ((fd2 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        int errno_cp = errno;
        close(patch_fd);
        close(fd1);
        errno = errno_cp;
        err(2, "Open failed!");
    }

    //copy f1 into f2 according to data_version types
    int w_status;
    if (h.data_version == 0x00)
    {
        uint8_t num;
        while ((r_status = read(fd1, &num, sizeof(uint8_t))) == sizeof(uint8_t))
        {
            if ((w_status = write(fd2, &num, sizeof(uint8_t))) != sizeof(uint8_t))
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd1);
                close(fd2);
                errno = errno_cp;
                err(6, "Write failed!");
            }
        }
        
        if (r_status == -1)
        {
            int errno_cp = errno;
            close(patch_fd);
            close(fd1);
            close(fd2);
            errno = errno_cp;
            err(3, "Read failed!");
        }
    }
    else
    {
        uint16_t word;
        while ((r_status = read(fd1, &word, sizeof(uint16_t))) == sizeof(uint16_t))
        {
            if ((w_status = write(fd2, &word, sizeof(uint16_t))) != sizeof(uint16_t))
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd1);
                close(fd2);
                errno = errno_cp;
                err(6, "Write failed!");
            }
        }

        if (r_status == -1)
        {
            int errno_cp = errno;
            close(patch_fd);
            close(fd1);
            close(fd2);
            errno = errno_cp;
            err(3, "Read failed!");
        }
    }

    //close f1 - not needed anymore
    close(fd1);

    //return to start of f2
    if (lseek(fd2, 0, SEEK_SET) == -1)
    {
        int errno_cp = errno;
        close(patch_fd);
        close(fd1);
        close(fd2);
        errno = errno_cp;
        err(7, "lseek failed!");
    }
   
    //check patch and f1 and make changes or exit
    if (h.data_version == 0x00)
    {
        struct pd
        {
            uint16_t offset;
            uint8_t o_byte;
            uint8_t n_byte;
        } d;

        uint8_t n;
        while ((r_status = read(patch_fd, &d, sizeof(d))) == sizeof(d))
        {
            if (lseek(fd2, d.offset, SEEK_SET) == -1)
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd2);
                errno = errno_cp;
                err(7, "lseek failed!");
            }

            int r_status2;
            if ((r_status2 = read(fd2, &n, sizeof(n))) != sizeof(n))
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd2);
                errno = errno_cp;
                err(3, "Read failed!");
            }

            if (n == d.o_byte)
            {
                if (lseek(fd2, -1, SEEK_CUR) == -1)
                {
                    int errno_cp = errno;
                    close(patch_fd);
                    close(fd2);
                    errno = errno_cp;
                    err(7, "lseek failed!");
                }

                if ((w_status = write(fd2, &d.n_byte, sizeof(d.n_byte))) != sizeof(d.n_byte))
                {
                    int errno_cp = errno;
                    close(patch_fd);
                    close(fd2);
                    errno = errno_cp;
                    err(6, "Write failed!");
                }
            }
            else
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd2);
                errno = errno_cp;
                err(8, "Invalid data!");
            }
        }
    }
    else
    {
        struct pd
        {
            uint32_t offset;
            uint16_t o_word;
            uint16_t n_word;
        } d;

        uint16_t w;
        while ((r_status = read(patch_fd, &d, sizeof(d))) == sizeof(d))
        {
            if (lseek(fd2, d.offset * 2, SEEK_SET) == -1)
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd2);
                errno = errno_cp;
                err(7, "lseek failed!");
            }

            int r_status2;
            if ((r_status2 = read(fd2, &w, sizeof(w))) != sizeof(w))
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd2);
                errno = errno_cp;
                err(3, "Read failed!");
            }

            if (w == d.o_word)
            {
                if (lseek(fd2, -2, SEEK_CUR) == -1)
                {
                    int errno_cp = errno;
                    close(patch_fd);
                    close(fd2);
                    errno = errno_cp;
                    err(7, "lseek failed!");
                }

                if ((w_status = write(fd2, &d.n_word, sizeof(d.n_word))) != sizeof(d.n_word))
                {
                    int errno_cp = errno;
                    close(patch_fd);
                    close(fd2);
                    errno = errno_cp;
                    err(6, "Write failed!");
                }
            }
            else
            {
                int errno_cp = errno;
                close(patch_fd);
                close(fd2);
                errno = errno_cp;
                err(8, "Invalid data!");
            }
        }
    }

    close(patch_fd);
    close(fd2);

    exit(0);
}
