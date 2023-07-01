#include <myhead.h>
int main()
{
    //创建一个socket，选择ipv4，tcp
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    printf("sockfd = %d\n", sockfd);

    close(sockfd);
}