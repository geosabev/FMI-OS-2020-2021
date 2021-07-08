#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc,char* argv[])
{
    char command[5];
    if(argc==1)
    {
        strcpy(command,"echo");
    }
    else if (argc==2)
    {
        if(strlen(argv[1])>4)
        {
            errx(1,"Invalid command");
        }
        strcpy(command,argv[1]);
    }
    else
    {
        errx(2,"Incorrect number of arguments");
    }
       
    while(1)
    {
        char string[5];
        for(int i=0;i<5;i++)
        {
            string[i]='\0';
        }
        int read_status;
        int i=0;
        char c;
        while((read_status=read(0,&c,sizeof(c)))>0)
        {
            if(c=='\n' || c==' ')
            {
                break;
            }
            if(i>=4)
            {
                errx(3,"Invalid string length");
            }
            string[i]=c;
            i++;
        }
        if(read_status==0)
        {
            exit(0);
        }
        if(read_status==-1)
        {
            err(4,"Read failed");
        }
        i=0;
        char string2[4];
        int rs;
        for(int i=0;i<4;i++)
        {
            string2[i]='\0';
        }
        while((rs=read(0,&c,sizeof(c)))>0)
        {
            if(c=='\n' || c==' ')
            {
                break;
            }
            if(i>=4)
            {
                errx(5,"Invalid string length");
            }
            string2[i]=c;
            i++;
        }
        if(rs==0)
        {
            if(execlp(command,command,string,(char*)NULL)==-1)
            {
                err(6,"Exec error");
            }
        }
        if(rs==-1)
        {
            err(7,"Read failed");
        }

        int pid=fork();
        if(pid==0)
        {
           if( execlp(command,command,string,string2,(char*)NULL)==-1)
           {
               err(8,"Exec error");
           }
        }
        else if(pid>0)
        {
            wait(NULL);
        }
        else
        {
            err(9,"Fork error");
        }
    }
}
