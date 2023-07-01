#include <myhead.h>

//socket bind listen
int tcpInit(int *pSockfd, const char *ip, const char *port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);

    int reuse = 1;
    int ret = -1;

    *pSockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*pSockfd, -1, "socket");

    ret = setsockopt(*pSockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(*pSockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(*pSockfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    return 0;
}