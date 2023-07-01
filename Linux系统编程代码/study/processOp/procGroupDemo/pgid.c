#include <myhead.h>
int main()
{
    if(fork() == 0)
    {
        printf("I am child process, pid = %d, pgid = %d\n", getpid(), getpgid(0));
        setpgid(0, 0);
        printf("I am child process, pid = %d, pgid = %d\n", getpid(), getpgid(0));
    }
    else 
    {
        printf("I am parent process, pid = %d, pgid = %d\n", getpid(), getpgid(0));
        wait(NULL);
    }
    
}