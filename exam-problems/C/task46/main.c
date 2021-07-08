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

    int n_flag = 0, current_argument = 1, row = 1;

    //-n argument
    if (strcmp(argv[current_argument], "-n") == 0)
    {
        current_argument++;
        n_flag = 1;
    }

    for (int i = current_argument; i < argc; i++)
    {
        //STDIN flag
        if (strcmp(argv[i], "-") == 0)
        {
            char c;
            int r_status, line_flag = 1;

            while ((r_status = read(0, &c, sizeof(c))) == sizeof(c))
            {
                if (n_flag == 1)
                {
                    if (line_flag == 1)
                    {
                        setbuf(stdout, NULL);
                        printf("\t%d ", row);
                        
                        if ((write(1, &c, sizeof(c))) != sizeof(c))
                        {
                            err(3, "Write failed!");
                        }

                        row++;
                        line_flag = 0;
                    }
                    else
                    {
                        if ((write(1, &c, sizeof(c))) != sizeof(c))
                        {
                            err(3, "Write failed!");
                        }
                    }
                    
                    if (c == '\n')
                    {
                        line_flag = 1;
                    }
                }
                else
                {
                    if ((write(1, &c, sizeof(c))) != sizeof(c))
                    {
                        err(3, "Write failed!");
                    }
                }
            }

            if (r_status == -1)
            {
                err(2, "Read failed!");
            }

            continue;            
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
        int r_status, line_flag = 1;

        while ((r_status = read(fd, &c, sizeof(c))) == sizeof(c))
        {
            if (n_flag == 1)
            {
                if (line_flag == 1)
                {
                    setbuf(stdout, NULL);
                    printf("\t%d ", row);

                    if ((write(1, &c, sizeof(c))) != sizeof(c))
                    {
                        err(3, "Write failed!");
                    }

                    row++;
                    line_flag = 0;
                }
                else
                {
                    if ((write(1, &c, sizeof(c))) != sizeof(c))
                    {
                        err(3, "Write failed!");
                    }
                }

                if (c == '\n')
                {
                    line_flag = 1;
                }
            }   
            else
            {
                if ((write(1, &c, sizeof(c))) != sizeof(c))
                {
                    err(3, "Write failed!");
                }
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

