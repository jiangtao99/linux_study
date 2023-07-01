#include <myhead.h>
int main(int argc, char *argv[])
{
    // ./listen 172.21.195.32 1234
    ARGS_CHECK(argc, 3);

    //创建一个socket，选择ipv4，tcp
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    printf("sockfd = %d\n", sockfd);

    ///创建时用sockaddr_in
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    //目标地址
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    //调用函数时，先取地址再强转
    //对于bind而言，addr一定是本地地址
    int ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    sleep(30);

    close(sockfd);
}