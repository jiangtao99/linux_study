#include <myhead.h>

int main(int argc, char *argv[])
{
    // ./chat2 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");
    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");
    puts("pipe open");

    char buf[4096] = {0};

    fd_set rdset;

    while(1)
    {
        //清空操作要放到循环里面
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr, &rdset);

        select(fdr+1, &rdset, NULL, NULL, NULL);

        if(FD_ISSET(fdr, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            //从管道中读取另一个进程发来的消息
            read(fdr, buf, sizeof(buf));
            printf("recv from chat22:%s\n", buf);
        }

        if(FD_ISSET(STDIN_FILENO, &rdset))
        {
            //从标准输入中读取消息后，通过管道回应消息
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            write(fdw, buf, strlen(buf));
        }
    }

    return 0;
}