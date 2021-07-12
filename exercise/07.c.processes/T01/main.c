#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

int main ()
{
	if (execl("/bin/date", "date", (char *)NULL) == -1)
    {
		err(99, "err execling");
	}

    exit(0);
}
