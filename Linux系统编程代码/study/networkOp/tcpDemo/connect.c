#include <myhead.h>
int main(int argc, char *argv[])
{
    // ./connect 172.21.195.32 1234
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
    int ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)); 
    ERROR_CHECK(ret, -1, "connect");

    char buf[4096] = {0};
    ret = recv(sockfd, buf, sizeof(buf), 0);
    puts(buf);

    ret = send(sockfd, "nihao from client", 17, 0);
    

    close(sockfd);
}