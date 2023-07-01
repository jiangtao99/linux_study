#include <myhead.h>
int main()
{

    int shmid = shmget(1000, 4096, IPC_CREAT|0600);
    ERROR_CHECK(shmid, -1, "shmget");
    struct shmid_ds stat;
    //获取状态
    int ret = shmctl(shmid, IPC_STAT, &stat);
    ERROR_CHECK(ret, -1, "shmctl IPC_STAT");
    printf("perm = %o\n", stat.shm_perm.mode);
    printf("size = %lu\n", stat.shm_segsz);
    
    stat.shm_perm.mode = 0666;
    ret = shmctl(shmid, IPC_SET, &stat);
    ERROR_CHECK(ret, -1, "shmctl IPC_SET");
}