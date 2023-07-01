#include <myhead.h>
// 服务端不让它从标准输入中输入数据了
// 只监听sockfd和netfd，即只负责建立连接和转发
// 实现了一个聊天室的代码
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
    int epfd = -1;
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
    // 服务端不再监听标准输入
    // 服务端的功能：处理新的客户端|监听sockfd
    // 处理客户端发送的消息的转发，维护netfd的数据然后进行转发
    epfd = epoll_create(1);

    struct epoll_event event;
    event.data.fd = sockfd;
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);

    // 10个netfd和一个sockfd
    struct epoll_event readyArr[11] = {0};

    while (1)
    {
        int readyNum = epoll_wait(epfd, readyArr, 11, -1);
        puts("epoll_wait returns!");

        for (int i = 0; i < readyNum; ++i)
        {
            // 遍历就绪事件队列
            if (readyArr[i].data.fd == sockfd)
            {
                netfdArr[curConn] = accept(sockfd, NULL, NULL);
                ERROR_CHECK(netfdArr[curConn], -1, "accept");

                event.data.fd = netfdArr[curConn];
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, netfdArr[curConn], &event);

                printf("new connect is accepted, curConn = %d\n", curConn);
                curConn++;
            }
            else
            {
                bzero(buf, sizeof(buf));
                ret = recv(readyArr[i].data.fd, buf, sizeof(buf), 0);
                for (int j = 0; j < curConn; ++j)
                {
                    if (readyArr[i].data.fd == netfdArr[j])
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