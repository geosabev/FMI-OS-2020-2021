#include <unistd.h>
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
#include <string.h>

int main(int argc, char* argv[])
{
	if ( argc != 2 )
    {
		errx(2,"One argument needed!");
	}

	int p_fd[2];

	if (pipe(p_fd) == -1)
    {
		err(1,"Pipe creation failed!");
	}

	int id = fork();

	if (id == -1)
    {
		err(2,"Fork failed!");
	}

    //child
	if (id == 0)
    {
		close(p_fd[1]);
		char c;

		while (read(p_fd[0], &c, sizeof(c)) > 0)
        {
			if (write(1, &c, sizeof(c)) != sizeof(c))
            {
				int errno_cp = errno;
				close(p_fd[0]);
				errno = errno_cp;
				err(3, "Failed to write to STDOUT!");
			}
		}

		close(p_fd[0]);

        exit(0);
	}
    //parent
	else
    {
		close(p_fd[0]);
		int len = strlen(argv[1]);
		
        if (write(p_fd[1], argv[1], len) != len)
        {
			int errno_cp = errno;
			close(p_fd[1]);
			errno = errno_cp;
			err(4, "Failed to write to pipe");
		}

		close(p_fd[1]);
		
        wait(NULL);
    }

    exit(0);
}
