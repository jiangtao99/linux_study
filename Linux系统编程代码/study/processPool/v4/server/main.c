#include <myhead.h>
#include "head.h"
#include "worker.h"

workerData_t *workerArr;
int workerNum;
void sigFunc(int signum)
{
    printf("signum = %d\n", signum);
    for(int i = 0; i < workerNum; ++i)
    {
        kill(workerArr[i].pid, SIGUSR1);
    }
    for(int i = 0; i < workerNum; ++i)
    {
        wait(NULL);
    }
    exit(0);
    puts("process pool is over!");
}

int main(int argc, char *argv[])
{
    // ./server 172.21.195.32 1234 3
    ARGS_CHECK(argc, 4);

    // 创建很多子进程
    workerNum = atoi(argv[3]);
    workerArr = (workerData_t *)calloc(workerNum, sizeof(workerData_t));
    makeChild(workerArr, workerNum);

    //父进程要注册信号,要先fork
    signal(SIGUSR1, sigFunc);

    // 初始化tcp连接
    int sockfd;
    tcpInit(&sockfd, argv[1], argv[2]);

    // 用epoll把tcp连接和子进程管理起来
    int epfd;
    epfd = epoll_create(1);
    epollAdd(sockfd, epfd);
    for(int i = 0; i < workerNum; ++i)
    {
        epollAdd(workerArr[i].pipeFd, epfd);
    }

    int listenSize = workerNum + 1;
    struct epoll_event *readyArr = (struct epoll_event *)calloc(listenSize, sizeof(struct epoll_event));
    while(1)
    {
        int readyNum = epoll_wait(epfd, readyArr, listenSize, -1);
        printf("epoll_wait ready!\n");
        for(int i = 0; i < readyNum; ++i)
        {
            if(readyArr[i].data.fd == sockfd)
            {
                puts("client connect");
                int netfd = accept(sockfd, NULL, NULL);
                ERROR_CHECK(netfd, -1, "accept");
                for(int j = 0; j < workerNum; ++j)
                {
                    if(workerArr[j].status == FREE)
                    {
                        printf("%d worker got a job, pid = %d\n", j, workerArr[j].pid);
                        sendFd(workerArr[j].pipeFd, netfd); //把网络链接移交给子进程
                        close(netfd);
                        workerArr[j].status = BUSY;
                        break;
                    }
                }
            }
            else
            {
                int j;
                for(j = 0; j < workerNum; ++j)
                {
                    //说明这个管道导致的epoll就绪,说明这个子进程干完活了
                    if(readyArr[i].data.fd == workerArr[j].pipeFd)
                    {
                        pid_t pid;
                        int ret = read(workerArr[j].pipeFd, &pid, sizeof(pid));
                        printf("%d worker fininsh his job\n", j);
                        workerArr[j].status = FREE;
                        break;
                    }
                }
            }
        }
    }
}