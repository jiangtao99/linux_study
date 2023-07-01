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

    fd_set rdset;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
    ERROR_CHECK(ret, -1, "recvfrom");

    puts(buf);
    printf("client ip = %s\n", inet_ntoa(clientAddr.sin_addr));
    printf("client port = %d\n", ntohs(clientAddr.sin_port));



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
                sendto(sockfd, "", 0, 0, (struct sockaddr*)&clientAddr, clientAddrLen);
                break;
            }
            sendto(sockfd, buf, ret, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
        }
        if(FD_ISSET(sockfd, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
            if(ret == 0)
            {
                puts("client close!");
                break;
            }
            puts(buf);
        }
        
    }

    close(sockfd);
}