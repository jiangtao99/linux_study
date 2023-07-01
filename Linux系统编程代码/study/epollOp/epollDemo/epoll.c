#include <myhead.h>
int main()
{   
    int epfd = -1;
    epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    printf("epfd = %d\n", epfd);

    close(epfd);
}