#include<stdlib.h>
#include<stdio.h>
#include<err.h>
#include<errno.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{	
	if (argc != 4) 
	{
		err(1, "Invalid arguments!");
	}

	int fid = fork();

	if(fid == 0)
	{
		//Child 1 
		int fid1 = fork();

		if (fid1 == 0)
		{
			//Child 2
			int fid2 = fork();

			if (fid2 == 0)
            {
			    //Child 3
       			if (execlp(argv[1], argv[1], (char*)NULL) == -1)
               	{
                    err(2, "First command failed to execute!");
               	}
			}
			else if (fid2 > 0)
            {
                //Child 2
				int status;

				wait(&status);

				if (WIFEXITED(status))
                {
					printf("PID: %d, EXIT_STATUS: %d\n", fid2, WEXITSTATUS(status));
				}

				if (execlp(argv[2], argv[2], (char*)NULL)  == -1)
                {
                    err(3, "Second command failed to execute!");
                }
			}
			else
            {
				err(4, "Third fork failed!");
			}
		}
		else if (fid1 > 0)
		{
            //Child 1
			int status;
            
            wait(&status);

            if (WIFEXITED(status))
            {
                printf("PID: %d, EXIT_STATUS: %d\n", fid1, WEXITSTATUS(status));
            }

			if (execlp(argv[3], argv[3], (char*)NULL) == -1)
            {
                err(5, "Third command failed to execute!");
            }
		}
		else
        {
			err(6,"Second fork failed!");
		}
	}
	else if (fid > 0)
	{
		int status;
        
        wait(&status);

        if (WIFEXITED(status))
        {
            printf("PID:%d, EXIT_STATUS: %d\n", fid, WEXITSTATUS(status));
        }
	}
	else
    {
		err(7, "First fork failed!");
	}

    exit(0);
}
