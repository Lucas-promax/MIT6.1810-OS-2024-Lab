#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2] , p2[2] ;
    if(pipe(p1)==-1 || pipe(p2)==-1) exit(1) ;
    char c ;
    
    int fd3 = dup(p1[0]) ; //fd3 -> p1 read
    int fd4 = dup(p2[1]) ; //fd4 -> p2 write
    write(fd4,(char*)'A',1) ; // ping
    
    int pid ;
    if((pid=fork())==-1)
    {
        exit(0) ;
    }
    else if(pid==0)
    {
        int fd3 = dup(p2[0]) ; //fd3 -> p2 read
        int fd4 = dup(p1[1]) ; //fd4 -> p1 write
        close(p1[0]) ;
        close(p1[1]) ;
        close(p2[0]) ;
        close(p2[1]) ;

        if(read(fd3,&c,1)>0)
        {
            char buf[20] = "4: received ping\n" ;
            write(1,buf,strlen(buf)) ;
            write(fd4,(char *)'A',1) ; //pong
        }
        else
        {
            exit(1) ;
        }

        exit(0) ;
    }

    close(p1[0]) ;
    close(p1[1]) ;
    close(p2[0]) ;
    close(p2[1]) ;

    if(wait(0)==-1) exit(1) ;
    if(read(fd3,&c,1)>0)
    {
        char buf[20] = "3: received pong\n" ;
        write(1,buf,strlen(buf)) ;
    }
    else
    {
        exit(1) ;
    }

    exit(0) ;
}