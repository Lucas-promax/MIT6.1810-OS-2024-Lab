#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

static int cmp(char* name, char* target) ;

static int cmp(char* name, char* target)
{
    return !strcmp(name,target) ;
}

static void search(char* pathname, char* target)
{
    int fd ;
    if((fd=open(pathname,O_RDONLY))<0)
    {
        fprintf(2, "ls: cannot open %s\n", pathname) ;
        return ;
    }

    struct dirent de ;
    
    if(strlen(pathname) + 1 + DIRSIZ + 1 > MAXPATH)
    {
      fprintf(2,"ls: path too long\n") ;
      close(fd) ;
      return ;
    }

    char buf[MAXPATH] ;
    memcpy(buf,pathname,strlen(pathname)) ;
    buf[strlen(pathname)] = '/' ;

    while(read(fd,&de,sizeof(de))==sizeof(de))
    {
        if(de.inum==0) continue ;
        char name[DIRSIZ+1] ;
        memcpy(name,de.name,DIRSIZ) ;
        name[DIRSIZ] = 0 ;
        if(strcmp(name,".")==0 || strcmp(name,"..")==0) continue;
        
        
        memcpy(buf+strlen(pathname)+1,name,DIRSIZ+1) ;
        
        int fd2 ;
        if((fd2=open(buf,O_RDONLY))==-1)
        {
            fprintf(2, "find: cannot open %s\n", buf) ;
            continue ;
        }
        struct stat st ;
        if(fstat(fd2,&st)<0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            close(fd2) ;
            continue ;
        }
        close(fd2) ;
        if(st.type==T_FILE)
        {
            if(cmp(name,target)) fprintf(1,"%s\n",buf) ;
        }
        else if(st.type==T_DIR)
        {
            search(buf,target) ;
        }
    }
    close(fd) ;
}

int main(int argc, char* argv[])
{
    if(argc<3)
    {
        fprintf(2,"usage: find dir file\n") ;
        exit(1) ;
    }
    
    int fd ;
    if((fd=open(argv[1],O_RDONLY))<0)
    {
        fprintf(2,"find: cannot open %s\n",argv[1]) ;
        exit(1) ;
    }
    
    struct stat st ;
    if(fstat(fd,&st)<0)
    {
        fprintf(2,"find: cannot stat %s\n",argv[1]) ;
        exit(1) ;
    }
    close(fd) ;
    
    if(st.type!=T_DIR)
    {
        fprintf(2,"usage: find dir file") ;
        exit(1) ;
    }

    search(argv[1],argv[2]) ;
    exit(0) ;
}