#include <myhead.h>
//计数型信号量
int main()
{
    int semid = semget(1001, 2, IPC_CREAT|0600);
    //key相同的信号量，长度必须相同
    ERROR_CHECK(semid, -1, "semget");

    //0号信号量表示鸡蛋
    //semctl(semid, 0, SETVAL, 10);
    //1号信号量表示面粉
    //semctl(semid, 1, SETVAL, 10);
    //可以用数组的方式SETALL
    unsigned short arr[2] = {10, 10};
    semctl(semid, 0, SETALL, arr);


    //int egg = semctl(semid, 0, GETVAL);
    //int flour = semctl(semid, 1, GETVAL);
    //printf("egg = %d, flour = %d\n", egg, flour);
    //可以用数组的方式GETALL
    unsigned short retArr[2];
    semctl(semid, 0, GETALL, retArr);
    printf("egg = %d, flour = %d\n", retArr[0], retArr[1]);

    //因为制作bread要影响到两个信号量，所以是[2]
    struct sembuf bread[2];
    struct sembuf cake[2];
    //制作面包消耗鸡蛋
    bread[0].sem_num = 0;
    bread[0].sem_op = -3;
    bread[0].sem_flg = SEM_UNDO;
    //制作面包消耗面粉
    bread[1].sem_num = 1;
    bread[1].sem_op = -2;
    bread[1].sem_flg = SEM_UNDO;


    //制作蛋糕面包消耗鸡蛋
    cake[0].sem_num = 0;
    cake[0].sem_op = -5;
    cake[0].sem_flg = SEM_UNDO;
    //制作蛋糕消耗面粉
    cake[1].sem_num = 1;
    cake[1].sem_op = -1;
    cake[1].sem_flg = SEM_UNDO;
    
    
    if(fork() == 0)
    {
        while(1)
        {
            printf("before make bread\n");
            printf("egg = %d, flour = %d\n", semctl(semid, 0, GETVAL), semctl(semid, 1, GETVAL));
            //填2是因为bread影响到两个资源
            semop(semid, bread, 2);
            printf("after make bread\n");
            printf("egg = %d, flour = %d\n", semctl(semid, 0, GETVAL), semctl(semid, 1, GETVAL));
            sleep(1);
        }
    }
    else
    {
        while(1)
        {
            printf("before make cake\n");
            printf("egg = %d, flour = %d\n", semctl(semid, 0, GETVAL), semctl(semid, 1, GETVAL));
            //填2是因为cake影响到两个资源
            semop(semid, cake, 2);
            printf("after make cake\n");
            printf("egg = %d, flour = %d\n", semctl(semid, 0, GETVAL), semctl(semid, 1, GETVAL));
            sleep(1);
        }
        wait(NULL);
    }
}