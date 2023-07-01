#include <myhead.h>
//如果对后面的网络传输，只需要把管道的文件描述符改为网络的文件描述符
int main()
{
    int fds[2];
    pipe(fds);
    // 父进程读文件，读完文件写管道
    // 父管道写端fds[1]开启，子管道读端fds[0]开启
    if(fork() == 0)
    {
        close(fds[1]);
        int fd = open("file2", O_RDWR|O_CREAT|O_TRUNC, 0666);
        ERROR_CHECK(fd, -1, "open");
        char buf[4096] = {0};
        while(1)
        {
            bzero(buf, sizeof(buf));
            int ret = read(fds[0], buf, sizeof(buf));
            //读管道和读文件表现是不一样的
            //没有数据时，读文件返回0，读管道阻塞

            if(ret == 0)
            {
                break;
            }
            write(fd, buf, ret);
        }
        close(fds[0]);
    }
    else
    {
        close(fds[0]);
        int fd = open("file1", O_RDWR);
        ERROR_CHECK(fd, -1, "open");
        char buf[4096] = {0};
        while(1)
        {
            bzero(buf, sizeof(buf));
            int ret = read(fd, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read");
            if(ret == 0)
            {
                break;
            }
            write(fds[1], buf, ret);
        }     
        //写完之后把管道关了
        close(fds[1]);
        wait(NULL);
    }
}