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
    int doneSize = 0;
    int slice = 0;
    int lastSize = 0;   //上一次显示的时候文件的大小
    recvn(sockfd, &dataLen, sizeof(int));
    recvn(sockfd, &fileSize, dataLen);
    printf("fileSize = %d\n", fileSize);

    slice = fileSize / 10000;

    time_t timeBeg, timeEnd;
    timeBeg = time(NULL);

    //接收文件内容
    while (1)
    {
        //先接受每次发来的文件内容长度
        //ret = recv(sockfd, &dataLen, sizeof(dataLen), MSG_WAITALL);
        ret = recvn(sockfd, &dataLen, sizeof(int));
        ERROR_CHECK(ret, -1, "recv");
        if(dataLen != 1000)
        {
            printf("dataLen = %d\n", dataLen);
        }

        doneSize += dataLen;
        if(doneSize - lastSize > slice)
        {
            printf("%5.2lf%%\r", 100.0 * doneSize / fileSize);
            fflush(stdout);
            lastSize = doneSize;
        }

        if(dataLen == 0)
        {
            printf("100.00%%\n");
            break;
        }
        //再接收每次发来的文件内容
        //ret = recv(sockfd, buf, dataLen, MSG_WAITALL);
        ret = recvn(sockfd, buf, dataLen);
        ERROR_CHECK(ret, -1, "recv");
    
        write(fd, buf, dataLen);
    }
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