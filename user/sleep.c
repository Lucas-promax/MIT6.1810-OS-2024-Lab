#include "kernel/types.h"
#include "user/user.h"
int main(int argc, char* argv[])
{
    if(argc<2) {
        write(2,"usage: sleep ticks\n", 19) ; 
        exit(1) ;
    }
    int ticks = atoi(argv[1]) ;
    sleep(ticks) ;
    exit(0) ;
}