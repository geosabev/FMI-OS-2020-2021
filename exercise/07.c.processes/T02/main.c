#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

int main (int argc, char* argv[])
{

	if (argc != 2)
    {
		errx(1, "One argument needed!");
	}

	if (execlp("/bin/ls", "ls", argv[1], 0) == -1)
    {
		err(2, "Error during execution!");
	}
}
