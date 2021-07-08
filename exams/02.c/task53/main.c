#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

const int DATA = 16384;
const int USER = 2048;

int main(int argc,char* argv[])
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

    struct log
    {
        uint32_t UID;
        uint16_t r1;
        uint16_t r2;
        uint32_t t1;
        uint32_t t2;
    } l;

    if (st.st_size % sizeof(l) != 0)
    {
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(4, "Invalid format!");
    }

    uint32_t values[DATA];
    uint32_t users[USER];
    uint32_t durations[USER];

    int exist = 0, log_count = 0, unique_count = 0;
    double avg, disp = 0;
    uint32_t dur;

    int r_status;
    while ((r_status = read(fd, &l, sizeof(l))) == sizeof(l))
    {
        dur = l.t2 - l.t1;

        exist = 0;

        for (int i = 0; i < unique_count; i++)
        {
            if (users[i] == l.UID)
            {
                if (durations[i] < dur)
                {
                    durations[i] = dur;
                }

                exist = 1;
            }
        }

        if (exist == 0)
        {
            users[unique_count] = l.UID;
            durations[unique_count] = dur;
            unique_count++;
        }

        log_count++;
        avg += dur;
        values[log_count] = dur;
    }

    if (r_status == -1)
    {  
        int errno_cp = errno;
        close(fd);
        errno = errno_cp;
        err(6, "Read failed!");
    }
    
    avg = avg / log_count;
    
    for (int i = 0; i < log_count; i++)
    {
        disp += ((values[i] - avg) * (values[i] - avg));
    }

    disp = disp / log_count;

    for (int i = 0; i < unique_count; i++)
    {
        if (durations[i] > disp)
        {
            printf("UID: %d, Longest session: %d\n", users[i], durations[i]);
        }
    }

    exit(0);
}

