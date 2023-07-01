#include <myhead.h>

typedef struct train_s
{
    /* data */
    int length;
    char buf[1000];
} train_t;

int recvn(int sockfd, void *pStart, int len)
{
    int total = 0;
    int ret = 0;
    char *p = (char *)pStart;
    //保证发送多少字节收到多少字节
    while(total < len)
    {
        ret = recv(sockfd, p+total, len-total, 0);
        total += ret;
    }    
    return 0;
}

int recvFile(int sockfd)
{
    // 先获取文件名
    int ret = -1;
    int fd = -1;
    int dataLen = 0;
    char name[1024] = {0};
    char buf[4096] = {0};

    //接收4个字节的数据长度
    //ret = recv(sockfd, &dataLen, sizeof(dataLen), MSG_WAITALL);
    ret = recvn(sockfd, &dataLen, sizeof(int));
    ERROR_CHECK(ret, -1, "recv");
    //接收文件名
    //ret = recv(sockfd, name, dataLen, MSG_WAITALL);
    ret = recvn(sockfd, name, dataLen);
    ERROR_CHECK(ret, -1, "recv");

    fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(fd, -1, "open");

    //获取文件长度
    int fileSize = 0;
    recvn(sockfd, &dataLen, sizeof(int));
    recvn(sockfd, &fileSize, dataLen);
    printf("fileSize = %d\n", fileSize);


    time_t timeBeg, timeEnd;
    timeBeg = time(NULL);

    //---- 文件接收部分 ----
    int pipefds[2];
    //建立管道，自己和自己进行通信，[0]读数据，[1]写数据
    //原因是客户端是接收数据，不能像服务端那样使用sendfile来接收数据（sendfile只能发送数据）

    pipe(pipefds);
    int total = 0;
    while(total < fileSize)
    {
        ret = splice(sockfd, NULL, pipefds[1], NULL, 4096, SPLICE_F_MORE);
        total += ret;
        usleep(10000);
        splice(pipefds[0], NULL, fd, NULL, ret, SPLICE_F_MORE);
    }

    //最后接收长度0
    recvn(sockfd, &dataLen, sizeof(int));
    printf("dataLen = %d\n", dataLen);

    //---- 文件接收部分 ----

    timeEnd = time(NULL);
    printf("total time = %ld\n", timeEnd - timeBeg);

    close(fd);
}

int main(int argc, char *argv[])
{
    // ./client 172.21.195.32 1234
    ARGS_CHECK(argc, 3);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = 0;
    int ret = 0;
    fd_set rdset;
    char buf[4096] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");

    recvFile(sockfd);

    /*
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sockfd, &rdset);
        select(sockfd+1, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            if(ret == 0)
            {
                send(sockfd, "client is over", 14, 0);
                break;
            }
            send(sockfd, buf, ret, 0);
        }
        if(FD_ISSET(sockfd, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = recv(sockfd, buf, sizeof(buf), 0);
            if(ret == 0)
            {
                break;
            }
            puts(buf);
        }
    }
    */
    close(sockfd);
}