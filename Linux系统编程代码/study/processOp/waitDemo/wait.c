#include <myhead.h>
int main()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        printf("I am child, pid = %d, ppid = %d\n", getpid(), getppid());
        //加while(1);让子进程不死，然后对子进程发生kill -9
        //while(1);
    }
    else 
    {
        printf("I am parent, pid = %d, ppid = %d\n", getpid(), getppid());
        //如果将sleep注释掉之后，子进程的ppid会出现问题，为1
        //sleep(1);
        //wait(NULL);
        int wstatus;
        wait(&wstatus);

        if(WIFEXITED(wstatus))
        {
            printf("Normal exit! Return value = %d\n", WEXITSTATUS(wstatus));
        }
        else if(WIFSIGNALED(wstatus))
        {
            printf("Abnormal exit! Terminal signal = %d\n", WTERMSIG(wstatus));
        }

    }
    return 0;
}