#include <myhead.h>

/*
    当前程序的小BUG，就是当票只是1时，一个线程抢到了锁，卖票，之后再补10张票。
    此时将唤醒等待条件变量的线程，然后自己解锁，若此时补票的线程并没有抢到锁，而是被卖票的线程抢到了，
    此时票依旧为0，并且flag为1，那么当买票线程发现票为0了，就退出了。
    即票还没补完，买票线程就终止了
*/

typedef struct
{
    int trainTicket;
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;

void *sellTicket1(void *arg)
{
    sleep(1); // 确保addTicket能够持有锁并wait
    shareRes_t *pShareRes = (shareRes_t *)arg;
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&pShareRes->mutex);
        if (pShareRes->trainTicket > 0)
        {
            printf("before 1 sell, ticket = %d\n", pShareRes->trainTicket);
            pShareRes->trainTicket--;
            printf("after 1 sell, ticket = %d\n", pShareRes->trainTicket);
            // 如果卖完票之后，没有票了，而且从来没有放过票
            if (pShareRes->trainTicket == 0 && pShareRes->flag == 0)
            {
                pthread_cond_signal(&pShareRes->cond);
                pShareRes->flag = 1;
            }
            else if (pShareRes->trainTicket == 0 && pShareRes->flag != 0)
            {
                // 如果卖完票之后，而且已经放过票了
                pthread_mutex_unlock(&pShareRes->mutex);
                break;
            }
        }
        else
        {
            pthread_mutex_unlock(&pShareRes->mutex);
            break;
        }
        pthread_mutex_unlock(&pShareRes->mutex);
    }
    pthread_exit(NULL);
}

void *sellTicket2(void *arg)
{
    sleep(1); // 确保addTicket能够持有锁并wait
    shareRes_t *pShareRes = (shareRes_t *)arg;
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&pShareRes->mutex);
        if (pShareRes->trainTicket > 0)
        {
            printf("before 2 sell, ticket = %d\n", pShareRes->trainTicket);
            pShareRes->trainTicket--;
            printf("after 2 sell, ticket = %d\n", pShareRes->trainTicket);
            // 如果卖完票之后，没有票了，而且从来没有放过票
            if (pShareRes->trainTicket == 0 && pShareRes->flag == 0)
            {
                pthread_cond_signal(&pShareRes->cond);
                pShareRes->flag = 1;
            }
            else if (pShareRes->trainTicket == 0 && pShareRes->flag != 0)
            {
                // 如果卖完票之后，而且已经放过票了
                pthread_mutex_unlock(&pShareRes->mutex);
                break;
            }
        }
        else
        {
            pthread_mutex_unlock(&pShareRes->mutex);
            break;
        }
        pthread_mutex_unlock(&pShareRes->mutex);
    }
    pthread_exit(NULL);
}

// 售票完毕之后才wait，意味着addTicket是wait
void *addTicket(void *arg)
{
    shareRes_t *pShareRes = (shareRes_t *)arg;
    pthread_mutex_lock(&pShareRes->mutex);
    // 表示不需要放票
    if (pShareRes->flag != 1)
    {
        pthread_cond_wait(&pShareRes->cond, &pShareRes->mutex);
    }
    printf("add ticket!\n");
    pShareRes->trainTicket = 10;
    pthread_mutex_unlock(&pShareRes->mutex);
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid1, tid2, tid3;
    shareRes_t shareRes;
    shareRes.trainTicket = 20; // 初始票的数量
    shareRes.flag = 0;         // 最开始不需要放票

    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    pthread_create(&tid1, NULL, sellTicket1, (void *)&shareRes);
    pthread_create(&tid2, NULL, sellTicket2, (void *)&shareRes);
    pthread_create(&tid3, NULL, addTicket, (void *)&shareRes);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);
}