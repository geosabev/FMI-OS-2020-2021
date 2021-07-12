#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "One argument needed!");
    }

    int fd;

    if ((fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
    {
        err(2, "Open failed!");
    }

    int pid = fork();

    if (pid == 0)
    {
        //child
        int w_status;

        if ((w_status = write(fd, "foobar", 6)) != 6)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(4, "Write failed!");
        }

        exit(0);
    }
    else if (pid > 0)
    {
        //parent
        int state;

        wait(&state);
        
        if (WIFEXITED(state))
        {
            lseek(fd, 0, SEEK_SET);

            int r_status, i = 0;
            char c;

            while ((r_status = read(fd, &c, 1)) != -1 && r_status != 0)
            {
                i++;

                if (i != 0 && i % 2 == 0)
                {
                    printf("%c ", c);
                }
                else
                {
                    printf("%c", c);
                }
            }

            close(fd);

            printf("\n");
        }
    }
    else
    {
        //failure
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(3, "Fork failed!");
    }

    exit(0);
}
