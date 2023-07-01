#include <myhead.h>
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int bufSize;
    socklen_t len = sizeof(bufSize);
    int ret = getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufSize, &len);
    ERROR_CHECK(ret, -1, "getsockopt");
    printf("bufSize = %d, len = %d\n", bufSize, len);


    bufSize = 4096;
    //但是最终结果会是8192，是因为内核帮我们把这个数字翻倍了
    //多出来的空间用来做bookkepping（记账）
    ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufSize, len);


    ret = getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &bufSize, &len);
    ERROR_CHECK(ret, -1, "getsockopt");
    printf("bufSize = %d, len = %d\n", bufSize, len);
}