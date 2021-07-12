#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>

int main()
{
	int fd1, fd2;
	char c;

	if ((fd1 = open("foo.txt", O_RDONLY)) == -1)
    {
        err(1, "Open failed!");
    }

	if ((fd2 = open("bar.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH) ) == -1 ) 
    {
		int errno_cp = errno;
		close(fd1);
        errno = errno_cp;
        err(1, "Open failed!");
	}

    int r_status, w_status;

	while ((r_status = read(fd1, &c, 1)) != 0 && r_status != -1 )
    {
		if ((w_status = write(fd2, &c, 1)) == -1)
        {
            break;
        }
	}

	close(fd1);
	close(fd2);

    if (r_status == -1)
    {
        errx(2, "Read error!");
    }

    if (w_status == -1)
    {
        errx(3, "Write error!");
    }

    exit(0);
}
