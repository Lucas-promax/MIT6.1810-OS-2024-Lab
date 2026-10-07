#include "kernel/types.h"
#include "user/user.h"

void primes(int) ; //__attribute__((noreturn));

/* pass the pipe's read end fd */
void primes(int fd)
{
    int pri , digit ;


    if(read(fd,&pri,4)>0) 
    {
        fprintf(1,"prime %d\n",pri) ;
    }
    else
    {
        close(fd) ;
        return ;
    }

    int p[2] ;
    if(pipe(p)==-1) exit(1) ;
    //此时的fd: fd(read from parent), p[0](下一层的读取源), p[1](write to child)

    int pid ;
    if((pid=fork())==-1) exit(1) ;
    else if(pid==0) // child process
    {
        close(fd) ;
        close(p[1]) ;
        primes(p[0]) ;
        exit(0) ;
    }

    //parent process
    close(p[0]) ;

    while(read(fd,&digit,4)>0)
    {
        if(digit%pri==0) continue;
        write(p[1],&digit,4) ;
    }
    close(fd) ;
    close(p[1]) ;

    while(wait(0)!=-1);

    return ;
}

int main()
{
    int p[2] ;
    if(pipe(p) == -1) exit(1);
    
    int pid ;
    if((pid=fork())==-1) exit(1) ;
    else if(pid==0) // child process
    {
        close(p[1]) ;
        primes(p[0]) ; 
        exit(0) ;
    }

    // parent process
    close(p[0]) ;
    for(int i=2 ; i<=280 ; i++)
        write(p[1],&i,4) ;
    close(p[1]) ;
    
    while(wait(0)!=-1) ;

    exit(0) ;
}