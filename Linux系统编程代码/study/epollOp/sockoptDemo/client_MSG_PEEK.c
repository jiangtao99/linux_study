#include <myhead.h>
int main(int argc, char *argv[])
{
    // ./client 172.21.195.32 1234
    ARGS_CHECK(argc, 3);

    int sockfd = -1;
    int ret = -1;
    char buf[4096] = {0};

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");

    send(sockfd, "hello", 5, 0);

    sleep(10);
    close(sockfd);

}