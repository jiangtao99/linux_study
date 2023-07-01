#include "worker.h"
#include "head.h"
int makeChild(workerData_t *workerArr, int workerNum)
{
    pid_t pid;
    int pipeFd[2];
    int ret = -1;

    for (int i = 0; i < workerNum; ++i)
    {
        //主进程和工作进程通过网络管道进行通信，目的是将任务（netfd）交给工作进程
        socketpair(AF_LOCAL, SOCK_STREAM, 0, pipeFd);
        pid = fork();
        if (pid == 0)
        {
            close(pipeFd[0]);
            // 子进程 确保不会脱离if结构
            eventHandler(pipeFd[1]);
        }
        // 父进程
        close(pipeFd[1]);
        printf("pipeFd = %d, pid = %d\n", pipeFd[0], pid);
        workerArr[i].pipeFd = pipeFd[0];
        workerArr[i].pid = pid;
        workerArr[i].status = FREE;
    }
}

int eventHandler(int pipeFd)
{
    while (1)
    {
        int netfd;
        pid_t pid = getpid();
        //阻塞在这，只有当子进程收到一个netfd时才开始执行任务
        recvFd(pipeFd, &netfd);
        // 后续的任务加在这里
        puts("I got a task!");
        //done
        transFile(netfd);

        printf("I have done this task!\n");
        
        //活干完了就往管道里面发送一个消息，父进程的epol_wait会就绪
        write(pipeFd, &pid, sizeof(pid));
        close(netfd);
        
    }
}