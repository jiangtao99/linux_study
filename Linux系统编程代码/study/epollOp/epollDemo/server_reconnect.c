#include <myhead.h>

//本代码只对单客户端有用

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

    int ret = -1;
    int sockfd = -1;
    int netfd = -1;
    int optval = 1;
    int epfd = -1;

    char buf[4096] = {0};


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 10);
    ERROR_CHECK(ret, -1, "listen");


    epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    //设置监听放在循环外面，因为epoll的就绪不会影响到监听集合，这样就不需要每一次循环开始把它放到监听集合里面
    struct epoll_event event;
    //表示把stdin以读事件的形式加入监听
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN;
    //把stdin以读事件加入监听
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    event.data.fd = sockfd;
    event.events = EPOLLIN;
    //把监听socket以读时间加入监听
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);

    //就绪事件数组多大，取决于你最多有可能多少个事件就绪
    struct epoll_event readyArr[3];
    int isConnected = 0;
    //上次发消息的时间，检查的时间
    time_t lastMsg, check;

    while(1)
    {
        int readyNum = epoll_wait(epfd, readyArr, 3, 1000);
        puts("epoll_wait returns");
        //如果处于已连接状态并且超时了
        if(readyNum == 0 && isConnected != 0)
        {
            puts("time out");
            check = time(NULL);
            if(check - lastMsg > 5)
            {
                send(netfd, "you are free", 12, 0);
                close(netfd);
                event.data.fd = netfd;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_DEL, netfd, &event);
                isConnected = 0;
            }
        }
        for(int i = 0; i < readyNum; ++i)
        {
            //如果事件就绪队列中有STDIN_FILENO
            if(readyArr[i].data.fd == STDIN_FILENO)
            {
                //如果没有连接的时候，服务端发送数据，当什么都不输入（ctrl+d）就终止服务端；如果输入，就什么都不做
                if(isConnected == 0)
                {
                    ret = read(STDIN_FILENO, buf, sizeof(buf));
                    if(ret == 0)
                    {
                        goto end;
                    }
                }
                bzero(buf, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                if(ret == 0)
                {
                    goto end;
                }
                send(netfd, buf, ret - 1, 0);
            }
            else if(readyArr[i].data.fd == sockfd)
            {
                //如果此时已经有连接，就不要再接收
                if(isConnected != 0)
                {
                    int nofd = accept(sockfd, NULL, NULL);
                    close(nofd);
                    continue;
                }
                netfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
                ERROR_CHECK(netfd, -1, "accept");
                event.data.fd = netfd;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &event);
                isConnected = 1;
                lastMsg = time(NULL);
                puts("client come");
            }
            else if(readyArr[i].data.fd == netfd)
            {
                bzero(buf, sizeof(buf));
                ret = recv(netfd, buf, sizeof(buf) - 1, 0);
                if(ret == 0)
                {
                    close(netfd);
                    event.data.fd = netfd;
                    event.events = EPOLLIN;
                    //移除监听
                    epoll_ctl(epfd, EPOLL_CTL_DEL, netfd, &event);  
                    isConnected = 0;
                }
                //lastMsg = time(NULL);
                //这里如果客户端使用ctrl+d退出，此时先会输出这个，信号后到
                puts(buf);
            }
        }
    }
end:
    close(netfd);
    close(epfd);
    close(sockfd);
}