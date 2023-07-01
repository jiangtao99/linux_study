/*
    实现一个简单的bash程序，2号信号到来时，打印当前的路径，可以创建子进程执行其他程序，
    父进程等待其他内容自己添加，比如输入exit就退出，ls命令等等。
*/
#include <myhead.h>

void sigFunc(int signum)
{
    printf("\n");
    printf("%s$ ", getcwd(NULL, 0));
    fflush(stdout);
}

int main()
{
    signal(SIGINT, sigFunc);
    while(1)
    {
        printf("%s$ ", getcwd(NULL, 0));
        fflush(stdout);
        char cmd[4096] = {0};
        read(STDIN_FILENO, cmd, sizeof(cmd));
        //输入时默认有一个换行，要把最后的换行符去掉
        //如touch file1\n
        cmd[strlen(cmd) - 1] = '\0';

        if(fork() == 0)
        {
            signal(SIGINT, SIG_DFL);
            char *argvector[100] = {0};
            argvector[0] = strtok(cmd, " ");
            int i = 1;
            while(1)
            {
                argvector[i] = strtok(NULL, " ");
                if(argvector[i] == NULL)
                {
                    break;
                }
                ++i;
            }
            //最后要以NULL结尾，正好strtok切到0
            execv(argvector[0], argvector);
        }
        else
        {

            wait(NULL);
        }
    }
}