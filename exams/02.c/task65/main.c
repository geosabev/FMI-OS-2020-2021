#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "Invalid arguments!");
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
        err(3, "Stat failed!");
    }

    if ((st.st_size % 16) != 0 || (st.st_size / 16) > 8)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(3, "Wrong format!");
    }

    struct log
    {
        char file[8];
        uint32_t offset;
        uint32_t length;
    } l;

    int r_status;
    uint16_t result = 0, child_result = 0;
    while ((r_status = read(fd, &l, sizeof(l))) == sizeof(l))
    {
        int p[2];

        if (pipe(p) == -1)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(5, "Pipe failed!");
        }
        
        int id = fork();

        if (id == -1)
        {
            int errno_cp = errno;
            close(fd);
            close(p[0]);
            close(p[1]);
            errno = errno_cp;
            err(6, "Fork failed!");
        }

        if (id == 0)
        {
            close(p[0]);

            int fd_f;
            if ((fd_f = open(l.file, O_RDONLY)) == -1)
            {
                int errno_cp = errno;
                close(fd);
                errno = errno_cp;
                err(2, "Open subfile failed!");
            }

            struct stat st1;
            if (fstat(fd_f, &st1) == -1)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_f);
                errno = errno_cp;
                err(3, "Stat failed!");
            }

            if (st1.st_size % sizeof(uint16_t) != 0)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_f);
                errno = errno_cp;
                err(3, "Wrong format!");
            }

            if (lseek(fd_f, l.offset * sizeof(uint16_t), SEEK_SET) == -1)
            {
                int errno_cp = errno;
                close(fd);
                close(fd_f);
                errno = errno_cp;
                err(3, "Lseek failed!");
            }

            uint16_t xor_result = 0;
            for (uint32_t i = 0; i < l.length; i++)
            {
                uint16_t el;
                if ((read(fd_f, &el, sizeof(uint16_t)) != sizeof(uint16_t)))
                {
                    int errno_cp = errno;
                    close(fd);
                    close(fd_f);
                    errno = errno_cp;
                    err(4, "Read failed!");
                }

                xor_result = xor_result ^ el;
            }

            if ((write(p[1], &xor_result, sizeof(uint16_t))) != sizeof(uint16_t))
            {
                int errno_cp = errno;
                close(fd);
                close(fd_f);
                close(p[1]);
                errno = errno_cp;
                err(6, "Write failed!");
            }
            
            close(fd_f);
            close(p[0]);

            exit(0);
        }
        
        close(p[1]);

        if ((read(p[0], &child_result, sizeof(uint16_t))) != sizeof(uint16_t))
        {
            int errno_cp = errno;
            close(fd);
            close(p[0]);
            errno = errno_cp;
            err(4, "Read failed!");
        }

        result = result ^ child_result;

        close(p[0]);
    }

    if (r_status == -1)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(4, "Read failed!");
    }

    printf("result: %dB\n", result);

    close(fd);

    exit(0);
}  
