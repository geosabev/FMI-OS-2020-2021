#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    char input[512];
    int r_status;

    while (1)
    {
        if ((write(1, "Enter command: ", 15)) != 15)
        {
            err(1, "Write failed!");
        }

        if ((r_status = read(0, input, sizeof(input))) == -1)
        {
            err(1, "Read failed!");
        }

        input[r_status - 1] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        int id = fork();
        if (id == -1)
        {
            err(2, "Fork failed!");
        }

        if (id == 0)
        {
            if (execlp(input, input, (char*)NULL) == -1)
            {
                err(3, "Execution of %s failed", input);
            }
        }

        int status;
        if (wait(&status) == -1)
        {
            err(4, "Could not wait for child!");
        }

        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) != 0)
            {
                errx(4, "Wait failed!");
            }
        }
    }
}
