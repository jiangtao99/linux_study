#include <myhead.h>
int main()
{
    // 共享内存创建时会自动初始化，全部为0
    int shmid = shmget(1000, 4096, IPC_CREAT|1000);
    ERROR_CHECK(shmid, -1, "shmget");
    char *p = (char *)shmat(shmid, NULL, 0);
    ERROR_CHECK(p, (void *)-1, "shmat");

    //strcpy(p, "hello");
    for(int i = 0; i < 5; i++)
    {
        printf("p[%d] = %c\n", i, p[i]);
    }

    shmdt(p);
}