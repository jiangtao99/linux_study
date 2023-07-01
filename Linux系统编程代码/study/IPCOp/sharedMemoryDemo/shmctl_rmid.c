#include <myhead.h>
int main()
{
    int shmid = shmget(1000, 4096, IPC_CREAT|0600);
    ERROR_CHECK(shmid, -1, "shmget");
        
    void *ret = shmat(shmid, NULL, 0);
    ERROR_CHECK(ret, (void *)-1, "shmat");

    //解除共享内存
    int iret = shmctl(shmid, IPC_RMID, NULL);
    ERROR_CHECK(iret, -1, "shmctl IPC_RMID");
    sleep(20);
    shmdt(ret);
}