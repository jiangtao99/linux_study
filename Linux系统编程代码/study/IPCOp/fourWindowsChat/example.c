#include <myhead.h>>

int p0, p1, p2, p3;
void sigFunc(int num)
{
    //kill(p0, 信号);
    //kill(p1, 信号);
    //kill(p2, 信号);
    //kill(p3, 信号);
    //发完之后把资源清理掉，关管道，删除共享内存...
}

int main()
{
    //四个进程的第一步都是这个
    int shmid = shmget(1000, 4096, IPC_CREAT|0600);
    int *pid = shmat(shmid, NULL, 0);
    //pid[0~3]对应四个进程的pid
    //把pid写到共享内存里面
    pid[0] = getpid();
    //陷入死循环，意思是如果任何一个进程没有把自己的pid写进去的话，
    //其他的所有进程都会进入死循环的操作，如果死循环能够退出的话说明四个都不为0
    //说明四个进程都创建好了
    while(pid[0] == 0 || pid[1] == 0 || pid[2] == 0 || pid[3] == 0) {}
    //把四个pid写到全局变量里面
    p0 = pid[0];
    p1 = pid[1];
    p2 = pid[2];
    p3 = pid[3];
    //接下来就写正常逻辑，a就创建一片共享内存和a1通信，...
    
    //当给任何一个进程发送SIGUSER1信号时，它要把这四个信号都要发走
    signal(SIGUSR1, sigFunc);
}