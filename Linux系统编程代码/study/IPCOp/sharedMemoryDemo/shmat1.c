#include <myhead.h>
#define NUM 10000000
int main()
{
    // 共享内存创建时会自动初始化，全部为0
    // IPC_PRIVATE是0
    int shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT|1000);
    ERROR_CHECK(shmid, -1, "shmget");
    //char *p = (char *)shmat(shmid, NULL, 0);
    int *p = (int *)shmat(shmid, NULL, 0);
    p[0] = 0;
    ERROR_CHECK(p, (void *)-1, "shmat");

    if(fork() == 0)
    {
        for(int i = 0; i < NUM; ++i)
        {
            ++p[0];
        }
    }
    else 
    {
        for(int i = 0; i < NUM; ++i)
        {
            ++p[0];
        }
        wait(NULL);
        printf("p[0] = %d\n", p[0]);
    }
    shmdt(p);
}