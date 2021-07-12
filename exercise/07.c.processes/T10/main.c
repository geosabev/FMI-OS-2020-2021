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

int main (int argc, char* argv[])
{
	if (argc != 3)
    {
		err(1, "Two arguments needed!");
	}

	int id=fork();

	if (id < 0)
    {
		err(2, "Fork failed!");
	}

	if (id == 0)
    {
	    if (execlp(argv[1], argv[1], (char*)NULL) == -1)
        {
		    err(3, "Failed to execute first command!");
		}
	}

	int status;

	if (wait(&status) == -1)
	{
		err(4, "Failed waiting for child process!");
	}

	if (WIFEXITED(status))
    {
	    if (WEXITSTATUS(status) == 0)
        {
		    if (execlp(argv[2], argv[2], (char*)NULL) == -1)
            {
		        err(5, "Failed to execute second command!");
		    }
	    }
	    else
        {
		    errx(6, "Previous command didn't work!");
	    }
	}

	err(42, "First command failed to execute!");

}
