#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

ssize_t contains(char c, char* set)
{
	ssize_t i = 0;
	
    while(set[i] != '\0')
    {
		if(set[i] == c)
        {
            return i;
        }

		i++;
	}

	return -1;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "Two arguments needed!");
    }

    int d_flag = 0, s_flag = 0;

    if (strcmp(argv[1], "-d") == 0)
    {
        d_flag = 1;
    }
    else if (strcmp(argv[1], "-s") == 0)
    {
        s_flag = 1;
    }
    else
    {
        if (strlen(argv[1]) != strlen(argv[2]))
        {
            errx(2, "Invalid arguments!");
        }
    }

    char buf;
    int r_status, w_status;

    //delete option
    if (d_flag)
    {
        while ((r_status = read(0, &buf, 1)) == 1)
        {
            if (contains(buf, argv[2]) == -1)
            {
                if ((w_status = write(1, &buf, 1)) != 1)
                {
                    err(4, "Write failed!");
                }
            }
        }

        if (r_status == -1)
        {
            err(3, "Read failed!");
        }

        exit(0);
    }

    //squeeze option
    if (s_flag)
    {
		char prev = '\0';

		while ((r_status = read(0, &buf, 1)) == 1)
        {
			if (contains(buf, argv[2]) == -1)
            {
				if ((w_status = write(1, &buf, 1)) != 1)
                {
					err(4, "Write failed!");
                }
            }
            else
            {
				if(buf != prev)
                {
					if((w_status = write(1, &buf, 1)) != 1)
                    {
                        err(4, "Write failed!");
                    }
                }
			}

			prev = buf;
		}

		if(r_status == -1)
        {
			err(3, "Read failed");
        }
        
        exit(0);
    }

    //two sets option
	while ((r_status = read(0, &buf, 1)) == 1)
    {
		ssize_t pos = contains(buf, argv[1]);

		if(pos != -1)
        {
			if((w_status = write(1, &argv[2][pos], 1)) != 1)
            {
				err(4, "Write failed!");
            }
        }
        
        if(pos == -1)
        {
			if((w_status = write(1, &buf, 1)) != 1)
            {
				err(4, "Write failed!");
            }
        }
    }

	if(r_status == -1)
    {
	    err(3, "Read failed!");
    }

	exit(0);
}
