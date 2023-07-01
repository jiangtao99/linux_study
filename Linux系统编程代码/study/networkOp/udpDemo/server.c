#include <myhead.h>
int main(int argc, char *argv[])
{
    // ./server 172.21.195.32 1234
    ARGS_CHECK(argc, 3);
    
    int sockfd = -1;
    int ret = -1;
    char buf[4096] = {0};
    struct sockaddr_in addr;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);


    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");


    ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
    ERROR_CHECK(ret, -1, "recvfrom");

    //printf("client ip = %s\n", inet_ntoa(clientAddr.sin_addr));
    //printf("client port = %d\n", ntohs(clientAddr.sin_port));
    puts(buf);

    bzero(buf, sizeof(buf));
    ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
    ERROR_CHECK(ret, -1, "recvfrom");

    puts(buf);

    ret = sendto(sockfd, "hello client", 12, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    ERROR_CHECK(ret, -1, "sendto");

    close(sockfd);
}