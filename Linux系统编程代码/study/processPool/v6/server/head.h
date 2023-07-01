int tcpInit(int *pSockfd, const char *ip, const char *port);
//发送一个文件描述符
int sendFd(int pipeFd, int fdToSend, int exitFlag);
//接收一个文件描述符
int recvFd(int pipeFd, int *pFdToRecv, int *pExitFlag);
int epollAdd(int fd, int epfd);
int epollDel(int fd, int epfd);
int transFile(int netfd);

typedef struct train_s
{
    /* data */
    int length;
    char buf[1000];
} train_t;
