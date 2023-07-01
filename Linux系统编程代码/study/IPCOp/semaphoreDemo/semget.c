#include <myhead.h>
#define NUM 10000000
int main()
{
    int shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT|0600);
    ERROR_CHECK(shmid, -1, "shmget");
    int *p = (int *)shmat(shmid, NULL, 0);
    ERROR_CHECK(p, (void *)-1, "shmat");
    p[0] = 0;

    //创建一个信号量，注意第二个是信号量的个数，使用时跟数组下标一样
    //使用二进制信号量的形式
    int semid = semget(1000, 1, IPC_CREAT|0600);    
    ERROR_CHECK(semid, -1, "semget");

    //设置信号量初始值
    //第二个参数是要操作的信号量的下标
    int ret = semctl(semid, 0, SETVAL, 1);
    ERROR_CHECK(semctl, -1, "semctl SETVAL");

    //获取第0号下标的信号量的值
    ret = semctl(semid, 0, GETVAL);
    ERROR_CHECK(ret, -1, "semctl GETVAL")

    printf("semval = %d\n", ret);

    //
    struct sembuf P, V;

    P.sem_num = 0;  //下标
    P.sem_op = -1;  //对资源的影响
    P.sem_flg = SEM_UNDO;    

    V.sem_num = 0;
    V.sem_op = 1;
    V.sem_flg = SEM_UNDO;

    if(fork() == 0)
    {
        for(int i = 0; i < NUM; ++i)
        {
            semop(semid, &P, 1);
            ++p[0];
            semop(semid, &V, 1);
        }
    }
    else
    {
        for(int i = 0; i < NUM; ++i)
        {
            semop(semid, &P, 1);
            ++p[0];
            semop(semid, &V, 1);
        }
        wait(NULL);
        printf("p[0] = %d\n", p[0]);
    }

    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);

}