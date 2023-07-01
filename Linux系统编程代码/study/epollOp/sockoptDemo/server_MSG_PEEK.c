#include <myhead.h>

int main(int argc, char *argv[])
{
    // ./server 172.21.195.32 1234
    ARGS_CHECK(argc, 3);

    int sockfd = -1;
    int netfd = -1;
    int ret = -1;
    char buf[4096] = {0};

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");
    
    ret = listen(sockfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    netfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    ERROR_CHECK(netfd, -1, "accept");

    bzero(buf, sizeof(buf));
    ret = recv(netfd, buf, sizeof(buf), MSG_PEEK);
    puts(buf);
    bzero(buf, sizeof(buf));
    ret = recv(netfd, buf, sizeof(buf), 0);
    puts(buf);

    close(netfd);
    close(sockfd);

}