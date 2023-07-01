#include <myhead.h>
/**
 * 因为read阻塞的行为，导致执行顺序是固定的，固定的实现是同步。
 * 
*/

int main()
{
    int pipefds1[2];
    int pipefds2[2];
    //两根管道，4个文件对象
    pipe(pipefds1);
    pipe(pipefds2);

    //pipefds1 父进程写 子进程读
    //pipefds2 父进程读 子进程写 
    if(fork() == 0)
    {
        //pipefds1 父写子读，子要关闭写：close(pipefds1[1])
        //pipefds2 父读子写，子要关闭读，close(pipefds2[0])
        close(pipefds1[1]);
        close(pipefds2[0]);

        char buf[1024] = {0};

        write(pipefds2[1], "msg from child", 14);

        read(pipefds1[0], buf, sizeof(buf));
        puts(buf);
    }
    else 
    {
        //pipefds1 父写子读，父要关闭读：close(pipefds1[0])
        //pipefds2 父读子写，父要关闭写，close(pipefds2[1])
        close(pipefds1[0]);
        close(pipefds2[1]);
        
        char buf[1024] = {0};
        read(pipefds2[0], buf, sizeof(buf));
        puts(buf);

        write(pipefds1[1], "msg from parent", 15);
        wait(NULL);

    }
}