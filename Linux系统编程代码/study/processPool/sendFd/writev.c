#include <myhead.h>
//使用writev可以发送碎片的文本数据到对端
int main()
{
    int fds[2];
    pipe(fds);
    if (fork() == 0)
    {
        // 关闭写端
        close(fds[1]);
        char buf[10] = {0};
        struct iovec iov[1];
        iov[0].iov_base = buf;
        iov[0].iov_len = sizeof(buf);

        readv(fds[0], iov, 1);
        printf("I am child, buf = %s\n", buf);

        exit(0);
    }
    else
    {
        //关闭读端
        close(fds[0]);
        char buf[10] = "hello";
        struct iovec iov[1];
        iov[0].iov_base = buf;
        iov[0].iov_len = strlen(buf);

        writev(fds[1], iov, 1);

        wait(NULL);
    }
}