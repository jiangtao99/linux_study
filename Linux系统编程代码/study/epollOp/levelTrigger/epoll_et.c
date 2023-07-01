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

    int ret = -1;
    int sockfd = -1;
    int netfd = -1;
    int optval = 1;
    int epfd = -1;

    char buf[5] = {0};


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    netfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    ERROR_CHECK(netfd, -1, "accept");

    epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    //设置监听放在循环外面，因为epoll的就绪不会影响到监听集合，这样就不需要每一次循环开始把它放到监听集合里面

    struct epoll_event event;
    //表示把stdin以读事件的形式加入监听
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN;
    //把stdin以读事件加入监听
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    event.data.fd = netfd;
    //event.events = EPOLLIN;
    //修改为边沿触发
    event.events = EPOLLIN|EPOLLET;
    
    //把已连接socket以读事件加入监听
    epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &event);

    //就绪事件数组多大，取决于你最多有可能多少个事件就绪
    struct epoll_event readyArr[2];

    while(1)
    {
        int readyNum = epoll_wait(epfd, readyArr, 2, -1);
        puts("epoll_wait returns");
        for(int i = 0; i < readyNum; ++i)
        {
            //如果事件就绪队列中有STDIN_FILENO
            if(readyArr[i].data.fd == STDIN_FILENO)
            {
                bzero(buf, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                if(ret == 0)
                {
                    goto end;
                }
                send(netfd, buf, ret - 1, 0);
            }
            else if(readyArr[i].data.fd == netfd)
            {
                bzero(buf, sizeof(buf));
                //ret = recv(netfd, buf, sizeof(buf) - 1, 0);
                //if(ret == 0)
                //{
                //    goto end;
                //}
                while(1)
                {
                    bzero(buf, sizeof(buf));
                    ret = recv(netfd, buf, sizeof(buf) - 1, MSG_DONTWAIT);
                    //阻塞的时候没数据返回0，非阻塞的时候没数据返回-1
                    if(ret == 0 || ret == -1)
                    {
                        break;
                    }
                    puts(buf);    

                }
                //puts(buf);
            }
        }
    }
end:
    close(netfd);
    close(epfd);
    close(sockfd);
}