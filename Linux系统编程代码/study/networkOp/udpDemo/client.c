#include <myhead.h>
int main(int argc, char *argv[])
{
    // ./client 172.21.195.32 1234
    ARGS_CHECK(argc, 3);
    
    int sockfd = -1;
    int ret = -1;
    char buf[4096] = {0};
    struct sockaddr_in addr;
    socklen_t sockLen = sizeof(addr);


    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = sendto(sockfd, "hello server", 12, 0, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "sendto");

    ret = sendto(sockfd, "world", 5, 0, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "sendto");

    ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &sockLen);
    ERROR_CHECK(ret, -1, "recvfrom");
    puts(buf);
    close(sockfd);
}