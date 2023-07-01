#include <myhead.h>
// 服务端不让它从标准输入中输入数据了
// 只监听sockfd和netfd，即只负责建立连接和转发
//实现了一个聊天室的代码
int main(int argc, char *argv[])
{
    // ./server 172.21.195.32 1234
    ARGS_CHECK(argc, 3);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int sockfd = 0;
    int ret = 0;
    // 单纯的去保存就绪的fd
    fd_set rdset;
    // 使用一个单独的监听集合
    fd_set monitorSet;
    char buf[4096] = {0};
    int optval = 1;
    // int netfd = -1;
    int netfdArr[10] = {0};
    int curConn = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    // netfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    // 实现聊天室
    //服务端不再监听标准输入
    //服务端的功能：处理新的客户端|监听sockfd
    //处理客户端发送的消息的转发，维护netfd的数据然后进行转发
    FD_ZERO(&monitorSet);
    FD_SET(sockfd, &monitorSet);

    while (1)
    {
        memcpy(&rdset, &monitorSet, sizeof(fd_set));

        select(20, &rdset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rdset))
        {
            netfdArr[curConn] = accept(sockfd, NULL, NULL);
            ERROR_CHECK(netfdArr[curConn], -1, "accept");
            FD_SET(netfdArr[curConn], &monitorSet);
            printf("new connect is accepted, curConn = %d\n", curConn);
            curConn++;
        }
        for (int i = 0; i < curConn; ++i)
        {
            if (FD_ISSET(netfdArr[i], &rdset))
            {
                bzero(buf, sizeof(buf));
                ret = recv(netfdArr[i], buf, sizeof(buf), 0);
                for(int j = 0; j < curConn; ++j)
                {
                    if(j == i)
                    {
                        continue;
                    }
                    send(netfdArr[j], buf, ret, 0);
                }
            }
        }
    }

    close(sockfd);
}