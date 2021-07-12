#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int fd, row_count = 0;
    char c;

    if (argc != 2) {
        write(2, "One argument needed!\n", 21); 
        exit(1);
    }

    if ( (fd = open(argv[1], O_RDONLY)) == -1 ) {
        write(2, "File failed to open correctly!\n", 31);
        exit(2);
    }

    int r_status, w_status;
    
    while ( (r_status = read(fd, &c, 1)) != 0 && r_status != -1 ) {
        if (c == '\n') {
            row_count = row_count + 1;
        }
        
        if ( (w_status = write(1, &c, 1)) == -1 ) {
            break;
        }

        if ( row_count == 10 ) {
            break;
        }
    }

    if (r_status == -1) {
        write(2, "Read error\n", 11);
        close(fd);
        exit(3);
    }

    if (w_status == -1) {
        write(2, "Write error\n", 12);
        close(fd);
        exit(4);
    }

    close(fd);

    exit(0);
}
