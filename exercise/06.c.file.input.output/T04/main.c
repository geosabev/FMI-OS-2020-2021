#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
	if (argc != 3)
    {
		errx(1, "Two arguments needed!");
	}

	int fd1;
	if ((fd1 = open(argv[1], O_RDWR)) ==  -1)
    {
		err(2, "Error while opening %s in read and write mode!", argv[1]);
	}

	int fd2;
	if ((fd2 = open(argv[2], O_RDWR)) == -1)
    {
		close(fd1);
		err(3, "Error while opening %s in read and write mode!", argv[2]);
	}

	int fd3;
    if ((fd3 = open("temp", O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR)) == -1)
    {
        close(fd1);
        close(fd2);
        err(4, "Error while opening temporary file in read and write mode!");
    }

	char c[4096];
    ssize_t s;

	while ((s = read(fd1, &c, sizeof(c))) != -1 && s != 0)
    {
		if (write(fd3, &c, s) != s )
        {
			close(fd1);
			close(fd2);
			close(fd3);

			err(1, "Writing error!");
		}
	}

	lseek(fd1, 0, SEEK_SET);

	while ((s = read(fd2, &c, sizeof(c))) != -1 && s != 0)
    {
		if (write(fd1, &c, s) != s)
        {
			close(fd1);
			close(fd2);
			close(fd3);
		
            err(1, "Writing error!");
		}
	}

	lseek(fd2, 0, SEEK_SET);
	lseek(fd3, 0, SEEK_SET);

	while ((s = read(fd3, &c, sizeof(c))) != -1 && s != 0)
    {
		if (write(fd2, &c, s) != s)
        {
			close(fd1);
			close(fd2);
			close(fd3);

			err(1, "Writing error!");
		}
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}
