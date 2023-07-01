#include <myhead.h>

int main(int argc, char *argv[])
{
    printf("fd of stdin = %d\n", fileno(stdin));

    printf("fd of stdout = %d\n", fileno(stdout));

    printf("fd of stderr = %d\n", fileno(stderr));

    //printf相当于是write(1,...)
    //write(1, "hello", 5);
    write(STDOUT_FILENO, "hello", 5);
    
    return 0;
}