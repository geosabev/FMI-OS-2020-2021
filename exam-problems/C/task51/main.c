#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int get_digit(char symbol)
{
    return (symbol - '0');
}

int main(int argc, char* argv[])
{
    //invalid use
    if (argc != 3 && argc != 5)
    {
        errx(1, "Invalid arguments!");
    }
    //cut characters
    else if (argc == 3)
    {
        if (strcmp(argv[1], "-c") == 0)
        {
            int start, end;

            //-c N
            if (strlen(argv[2]) == 1 && (argv[2][0] >= '0' && argv[2][0] <= '9'))
            {
                start = get_digit(argv[2][0]);
                end = start;
            }
            //-c N-M
            else if (strlen(argv[2]) == 3 && (argv[2][0] >= '0' && argv[2][0] <= '9') && argv[2][1] == '-' && (argv[2][2] >= '0' && argv[2][2] <= '9'))
            {
                start = get_digit(argv[2][0]);
                end = get_digit(argv[2][2]);
            }
            //invaid usage
            else
            {
                errx(1, "Invalid usage of -c option!");
            }

            if (start > end)
            {
                errx(1, "Invalid interval!");
            }

            char c;
            int r_status, count = 0;
            while ((r_status = read(0, &c, sizeof(c))) == sizeof(c))
            {
                count++;

                if (c == '\n')
                {
                    if ((write(1, "\n", 1)) != 1)
                    {
                        err(3, "Write failed!");
                    }

                    count = 0;
                }
                else if (count >= start && count <= end)
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
        }
        else
        {
            errx(1, "Invalid arguments!");
        }
    }
    //delimiter option
    else
    {
        if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-f") == 0)
        {
            char delim = argv[2][0];
            int start, end;

            //-f N
            if (strlen(argv[4]) == 1 && (argv[4][0] >= '0' && argv[4][0] <= '9'))
            {
                start = get_digit(argv[4][0]);
                end = start;
            }
            //-f N-M
            else if (strlen(argv[4]) == 3 && (argv[4][0] >= '0' && argv[4][0] <= '9') && argv[4][1] == '-' && (argv[4][2] >= '0' && argv[4][2] <= '9'))
            {
                start = get_digit(argv[4][0]);
                end = get_digit(argv[4][2]);
            }
            //invaid usage
            else
            {
                errx(1, "Invalid usage of -f option!");
            }

            if (start > end)
            {
                errx(1, "Invalid interval!");
            }

            char c;
            int r_status, count = 0;
            while ((r_status = read(0, &c, sizeof(c))) == sizeof(c))
            {
                if (c == delim || count == 0)
                {
                    count++;
                }

                if (c == '\n')
                {
                    if ((write(1, "\n", 1)) != 1)
                    {
                        err(3, "Write failed!");
                    }

                    count = 0;
                }
                else if (count >= start && count <= end)
                {
                    if (((count == start) && (c == delim)) == 0)
                    {
                        if ((write(1, &c, sizeof(c))) != sizeof(c))
                        {
                            err(3, "Write failed!");
                        }
                    }
                }
            }

            if (r_status == -1)
            {
                err(2, "Read failed!");
            }
        }
        else
        {
            errx(1, "Invalid arguments!");
        }
    }

    exit(0);
}
