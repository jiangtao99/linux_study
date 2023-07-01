#include <myhead.h>

int main(int argc, char *argv[])
{
    // ./chat1 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");
    puts("pipe open");

    char buf[4096] = {0};
    int ret = 0;
    fd_set rdset;

    while(1)
    {
        //清空操作要放到循环里面
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr, &rdset);

        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 500000;

        ret = select(fdr+1, &rdset, NULL, NULL, &timeout);
        //超时导致的就绪
        if(ret == 0)
        {
            printf("time out!\n");
            continue;
        }

        if(FD_ISSET(fdr, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            //从管道中读取另一个进程发来的消息
            ret = read(fdr, buf, sizeof(buf));
            if(ret == 0)
            {
                printf("end!\n");
                break;
            }
            
            printf("recv from B:%s\n", buf);
        }

        if(FD_ISSET(STDIN_FILENO, &rdset))
        {
            //从标准输入中读取消息后，通过管道回应消息
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            //此时发送ctrl+d
            if(ret == 0)
            {
                write(fdw, "you are a good person!", 22);
                break;
            }
            write(fdw, buf, strlen(buf));
        }
    }
    
    printf("发送的是ctrl+d\n");

    return 0;
}