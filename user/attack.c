#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"
#define MAXSCAN 32

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  
  char *end = sbrk(PGSIZE*MAXSCAN) ;
  for(int i=0 ; i<MAXSCAN ; i++){
    char str[33] ;
    memset(str,0,sizeof(str)) ;
    memcpy(str,end+8,24) ;
    if(strcmp(str,"very very secret pw is: ")==0)
      break;
    end += PGSIZE ;
  }
  write(2,end+32,8) ;

  exit(1);
}
