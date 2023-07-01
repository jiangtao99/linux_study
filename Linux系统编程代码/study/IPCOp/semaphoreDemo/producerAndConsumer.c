#include <myhead.h>
#define NUM 10000000
int main()
{
    int shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0600);
    ERROR_CHECK(shmid, -1, "shmget");
    int *p = (int *)shmat(shmid, NULL, 0);
    ERROR_CHECK(p, (void *)-1, "shmat");

    // p[0]表示仓库个数
    p[0] = 10;
    // p[1]表示商品的个数
    p[1] = 0;

    // 创建一个信号量，注意第二个是信号量的个数，使用时跟数组下标一样
    // 使用二进制信号量的形式
    int semid = semget(1000, 1, IPC_CREAT | 0600);
    ERROR_CHECK(semid, -1, "semget");

    // 设置信号量初始值
    // 第二个参数是要操作的信号量的下标
    int ret = semctl(semid, 0, SETVAL, 1);
    ERROR_CHECK(semctl, -1, "semctl SETVAL");

    // 获取第0号下标的信号量的值
    ret = semctl(semid, 0, GETVAL);
    ERROR_CHECK(ret, -1, "semctl GETVAL")

    printf("semval = %d\n", ret);

    //
    struct sembuf P, V;

    P.sem_num = 0; // 下标
    P.sem_op = -1; // 对资源的影响
    P.sem_flg = SEM_UNDO;

    V.sem_num = 0;
    V.sem_op = 1;
    V.sem_flg = SEM_UNDO;

    if (fork() == 0)
    {
        while (1)
        {
            semop(semid, &P, 1);
            if (p[0] > 0)
            {
                printf("before produce, space = %d, good = %d\n", p[0], p[1]);
                --p[0];
                ++p[1];
                printf("after produce, space = %d, good = %d\n", p[0], p[1]);
            }
            semop(semid, &V, 1);
            // sleep(1);
        }
    }
    else if (fork() == 0)
    {
        while (1)
        {
            semop(semid, &P, 1);
            if (p[0] > 0)
            {
                printf("before produce, space = %d, good = %d\n", p[0], p[1]);
                --p[0];
                ++p[1];
                printf("after produce, space = %d, good = %d\n", p[0], p[1]);
            }
            semop(semid, &V, 1);
            // sleep(1);
        }
    }
    else
    {
        while (1)
        {
            semop(semid, &P, 1);
            if (p[1] > 0)
            {
                printf("before consume, space = %d, good = %d\n", p[0], p[1]);
                --p[1];
                ++p[0];
                printf("after consume, space = %d, good = %d\n", p[0], p[1]);
            }
            semop(semid, &V, 1);
            // sleep(2);
        }
        wait(NULL);
    }

    shmdt(p);
    shmctl(shmid, IPC_RMID, NULL);
}