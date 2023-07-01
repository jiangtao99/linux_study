#include <myhead.h>
int main()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        printf("I am child, pid = %d, ppid = %d\n", getpid(), getppid());
    }
    else 
    {
        printf("I am parent, pid = %d, ppid = %d\n", getpid(), getppid());
        //如果将sleep注释掉之后，子进程的ppid会出现问题，为1
        //sleep(1); 
    }
    return 0;
}