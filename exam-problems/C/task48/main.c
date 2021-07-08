#include <sys/types.h>                    
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    //stdin directly
    if (argc == 1)
    {
        char c;
        int r_status;
        
        while ((r_status = read(0, &c, sizeof(c))) == sizeof(c))
        {
            if ((write(1, &c, sizeof(c))) == -1)
            {
                err(2, "Write failed!");
            }
        }

        if (r_status == -1)
        {
            err(1, "Read failed!");
        }
        
        exit(0);
    }

    for (int i = 1; i < argc; i++)
    {
        //STDIN flag
        if (argv[i][0] == '-')
        {
            char c;
            int r_status;

            while ((r_status = read(0, &c, sizeof(c))) == sizeof(c))
            {
                if ((write(1, &c, sizeof(c))) != sizeof(c))
                {
                    err(3, "Write failed!");
                }

                if (r_status == -1)
                {   
                    err(2, "Read failed!");
                }

                continue;            
            }
        }

        //file
        int fd;
        if ((fd = open(argv[i], O_RDONLY)) == -1)
        {
            err(1, "Open failed!");
        }

        struct stat st;
        if (fstat(fd, &st) == -1)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(4, "Stat failed!");
        }

        char c;
        int r_status;

        while ((r_status = read(fd, &c, sizeof(c))) == sizeof(c))
        {
            if ((write(1, &c, sizeof(c))) != sizeof(c))
            {
                    err(3, "Write failed!");
            }
        }

        if (r_status == -1)
        {
            int errno_cp = errno;
            close(fd);
            errno = errno_cp;
            err(2, "Read failed!");
        }

        close(fd);
    }

    exit(0);
}

