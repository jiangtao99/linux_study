#include <myhead.h>
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
    close(sockfd);
}