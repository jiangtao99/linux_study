#include <myhead.h>
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
    //单纯的去保存就绪的fd
    fd_set rdset;
    //使用一个单独的监听集合
    fd_set monitorSet;
    char buf[4096] = {0};
    int netfd = -1;
    int optval = 1;



    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    //netfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    //1.accept要放在select后面
    //2.去使用时确保从标准输入中输入数据在客户端建立连接之后
    //3.每一次accept之后创建新的netfd，这个netfd要加入监听--->分离监听和就绪
    //4.客户端如果断开连接以后，服务端不要退出，要取消监听netfd
    FD_ZERO(&monitorSet);
    FD_SET(STDIN_FILENO, &monitorSet);
    FD_SET(sockfd, &monitorSet);

    while(1)
    {
        memcpy(&rdset, &monitorSet, sizeof(fd_set));

        select(20, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(sockfd, &rdset))
        {
            netfd = accept(sockfd, NULL, NULL);
            ERROR_CHECK(netfd, -1, "accept");
            FD_SET(netfd, &monitorSet);
            puts("new connect is accepted!");
        }
        if(FD_ISSET(STDIN_FILENO, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            if(ret == 0)
            {
                break;
            }
            send(netfd, buf, ret, 0);
        }
        if(FD_ISSET(netfd, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = recv(netfd, buf, sizeof(buf), 0);
            if(ret == 0)
            {
                puts("client is end!");
                FD_CLR(netfd, &monitorSet);
                close(netfd);
                netfd = -1;
                continue;
            }
            puts(buf);
        }        
    }   


    close(netfd);
    close(sockfd);
}