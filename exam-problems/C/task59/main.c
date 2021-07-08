#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

// cut -d: -f 7 /etc/passwd | sort | uniq -c | sort -n -k 1

int main()
{
    //sort -n -k 1
    int p1[2];

    if (pipe(p1) == -1)
    {
        err(4, "First pipe failed!");
    }

    int id1 = fork();

    if (id1 == -1)
    {
        close(p1[0]);
        close(p1[1]);
        err(1, "Sort #1 fork failed!");
    }

    //child
    if (id1 == 0)
    {
        //uniq -c
        close(p1[0]);
        dup2(p1[1], 1);
        close(p1[1]);

        int p2[2];

        if (pipe(p2) == -1)
        {
            err(4, "Second pipe failed!");
        }

        int id2 = fork();

        if (id2 == -1)
        {
            close(p2[0]);
            close(p2[1]);
            err(1, "Uniq fork failed!");
        }

        //child
        if (id2 == 0)
        {
            //sort
            close(p2[0]);
            dup2(p2[1], 1);
            close(p2[1]);

            int p3[2];

            if (pipe(p3) == -1)
            {
                err(4, "Third pipe failed!");
            }

            int id3 = fork();

            if (id3 == -1)
            {
                close(p3[0]);
                close(p3[1]);
                err(1, "Sort #2 fork failed!");
            }

            //child
            if (id3 == 0)
            {
                //cut -d : -f 7 /etc/passwd
                close(p3[0]);
                dup2(p3[1], 1);
                close(p3[1]);

                if (execlp("cut", "cut", "-d", ":", "-f", "7", "/etc/passwd", (char*)NULL) == -1)
                {
                    err(5, "Failed to execute cut!");
                }
            }

            //parent: sort
            close(p3[1]);
            dup2(p3[0], 0);
            close(p3[0]);

            if (execlp("sort", "sort", (char*)NULL) == -1)
            {
                err(5, "Failed to execute first sort!");
            }
        }

        //parent: uniq -c
        close(p2[1]);
        dup2(p2[0], 0);
        close(p2[0]);

        if (execlp("uniq", "uniq", "-c", (char*)NULL) == -1)
        {
            err(5, "Failed to execute uniq!");
        }
    }

    //parent: sort -n -k 1
    close(p1[1]);
    dup2(p1[0], 0);
    close(p1[0]);

    if (execlp("sort", "sort", "-n", "-k", "1", (char*)NULL) == -1)
    {
        err(5, "Failed to execute second sort!");
    }

    exit(0);
}
