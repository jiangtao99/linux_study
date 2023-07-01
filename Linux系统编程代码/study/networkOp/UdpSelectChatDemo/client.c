#include <myhead.h>
int main(int argc, char *argv[])
{
    // ./client 172.21.195.32 1234
    ARGS_CHECK(argc, 3);

    int sockfd = -1;
    int ret = -1;
    char buf[4096] = {0};
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    fd_set rdset;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);    
    ERROR_CHECK(sockfd, -1, "socket");

    ret = sendto(sockfd, "client come!", 12, 0, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "sendto");

    while(1)
    {
        bzero(buf, sizeof(buf));
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
                sendto(sockfd, "", 0, 0, (struct sockaddr *)&addr, sizeof(addr));
                break;
            }
            sendto(sockfd, buf, ret, 0, (struct sockaddr *)&addr, sizeof(addr));
        }
        if(FD_ISSET(sockfd, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrLen);
            if(ret == 0)
            {
                puts("server close!");
                break;
            }
            puts(buf);
        }
    }

    close(sockfd);
}