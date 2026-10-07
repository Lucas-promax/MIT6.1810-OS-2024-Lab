#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int argnum = 0 ;
char* args[MAXARG] ;
char arg[100] ;

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        fprintf(2,"usage: xargs cmd [arg1] [arg2]...") ;
        exit(1) ;
    }
    else if(argc-1>MAXARG)
    {
        fprintf(2,"xargs: too many arguments.\n") ;
        exit(1) ;
    }
    for(int i=1 ; i<argc ; i++)
        args[argnum++] = argv[i] ;
    
    char c ;

    int cnt = 0 ;
    while(read(0,&c,1)==1)
    {
        if(c!='\n')
        {
            if(cnt+1>=100)
            {
                fprintf(2,"xargs: argument too long.\n") ;
                exit(1) ;
            }
            arg[cnt++] = c ;
        }
        else
        {
            arg[cnt] = 0 ;
            cnt = 0 ;
            if(argnum+1>=MAXARG)
            {
                fprintf(2,"xargs: too many arguments\n") ;
                exit(1) ;
            }
            args[argnum++] = arg ;
            args[argnum] = 0 ;
            int pid ;
            if((pid = fork())==-1)
            {
                fprintf(2,"xargs: fork failed.\n") ;
                exit(1) ;
            }
            else if(pid==0)
            {
                exec(args[0],args) ;
                fprintf(2,"xargs: exec %s failed.\n",args[0]) ;
                exit(1) ;
            }
            wait(0) ;
            argnum-- ;
        }
    }
    exit(0) ;
}