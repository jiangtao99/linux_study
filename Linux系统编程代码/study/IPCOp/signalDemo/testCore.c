//证明core文件是由接受信号SIGSEGV产生的

/**
 * gcc testCore.c -o testCore -g
 * gdb testCore
 * r
 * Program received signal SIGSEGV, Segmentation fault.
*/
#include <myhead.h>

int main()
{
    /*
    int *p = 1234;
    *p = 1;
    */
}