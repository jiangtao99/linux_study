#include <myhead.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDWR);
    int fdw = open(argv[1], O_RDWR);

    printf("pipe open\n");
    char buf[4096];

    fd_set rdset;
    fd_set wrset;
    int cnt = 0;
    //一开始管道中没有数据，所以这个管道是可写的
    //让读和写管道的速度不一致，测试是否会造成阻塞
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(fdr, &rdset);

        FD_ZERO(&wrset);
        FD_SET(fdw, &wrset);
    
        select(fdw+1, &rdset, &wrset, NULL, NULL);
        if(FD_ISSET(fdr, &rdset))
        {
            printf("read cnt = %d\n", cnt++);
            read(fdr, buf, 2048);            
        }

        if(FD_ISSET(fdw, &wrset))
        {
            printf("write cnt = %d\n", cnt++);
            write(fdw, buf, 4097);
        }
        sleep(1);
    }

    return 0;
}