#include <myhead.h>
int main()
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        printf("I am child, pid = %d, ppid = %d\n", getpid(), getppid());
        // 加while(1);让子进程不死，然后对子进程发生kill -9
        //while(1);
        sleep(5);
    }
    else
    {
        printf("I am parent, pid = %d, ppid = %d\n", getpid(), getppid());
        // 如果将sleep注释掉之后，子进程的ppid会出现问题，为1
        // sleep(1);
        // wait(NULL);
        int wstatus;

        // 这样设置和wait一样
        // waitpid(-1, &wstatus, 0);
        while (1)
        {
            int ret = waitpid(-1, &wstatus, WNOHANG);
            if (ret == 0)
            {
                printf("child process has not yet dead!\n");
                sleep(1);
            }
            else
            {
                if (WIFEXITED(wstatus))
                {
                    printf("Normal exit! Return value = %d\n", WEXITSTATUS(wstatus));
                }
                else if (WIFSIGNALED(wstatus))
                {
                    printf("Abnormal exit! Terminal signal = %d\n", WTERMSIG(wstatus));
                }
                //当这个子进程真正死了后循环才退出
                break;
            }
        }
    }
    return 0;
}