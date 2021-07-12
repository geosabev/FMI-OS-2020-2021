#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    int fd, l_count = 0, w_count = 0, c_count = 0;
    char c;

    if (argc != 2)
    {
        write(2, "One argument!\n", 14);
        exit(1); 
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        write(2, "Open failed!\n", 13);
        exit(2);
    }

    int r_status;

    while ((r_status = read(fd, &c, 1)) != -1 && r_status != 0)
    {
        if (c == '\n')
        {
            w_count++;
            l_count++;
        }
        
        if (c == ' ')
        {
            w_count++;
        }

        c_count++;
    }

    printf("%d %d %d %s\n", l_count, w_count, c_count, argv[1]);
    
    close(fd);

    exit(0);
}
