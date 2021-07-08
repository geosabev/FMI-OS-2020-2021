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
// find ~ -type f -printf "%f %T@\n" | sort -nr -t ' ' -k2 | cut -d ' ' -f 1 | head -n 1

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(1, "Invalid arguments!");
    }

    //head -n 1
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
        err(1, "Second fork failed!");
    }

    //child
    if (id1 == 0)
    {
        //cut -d ' ' -f 1
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
            err(1, "Third fork failed!");
        }

        //child
        if (id2 == 0)
        {
            //sort -nr -t ' ' -k 2
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
                err(1, "Fourth fork failed!");
            }

            //child
            if (id3 == 0)
            {
                //find <dir> -type f -printf "%f %T@\n" 2>/dev/null
                close(p3[0]);
                dup2(p3[1], 1);
                close(p3[1]);

                if (execlp("find", "find", argv[1], "-type", "f", "-printf", "%p %T@\n", (char*)NULL) == -1)
                {
                    err(5, "Failed to execute find!");
                }
            }

            //parent: sort -nr -t ' ' -k 2
            close(p3[1]);
            dup2(p3[0], 0);
            close(p3[0]);

            if (execlp("sort", "sort", "-nr", "-k2", (char*)NULL) == -1)
            {
                err(5, "Failed to execute sort!");
            }
        }

        //parent: cut -d ' ' -f 1
        close(p2[1]);
        dup2(p2[0], 0);
        close(p2[0]);

        if (execlp("cut", "cut", "-d", " ", "-f1", (char*)NULL) == -1)
        {
            err(5, "Failed to execute cut!");
        }
    }

    //parent: head -n 1
    close(p1[1]);
    dup2(p1[0], 0);
    close(p1[0]);

    if (execlp("head", "head", "-n", "1", (char*)NULL) == -1)
    {
        err(5, "Failed to execute second sort!");
    }

    exit(0);
}
