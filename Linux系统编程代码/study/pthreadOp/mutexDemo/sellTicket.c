#include <myhead.h>
typedef struct shareRes_s
{
    int trainTicket;
    pthread_mutex_t mutex;
} shareRes_t;

void *sellTicket1(void *arg)
{
    int cnt = 0;
    shareRes_t *pShareRes = (shareRes_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&pShareRes->mutex);
        if (pShareRes->trainTicket <= 0)
        {
            // 在所有离开循环的场景，记得解锁
            pthread_mutex_unlock(&pShareRes->mutex);
            break;
        }

        //printf("before 1 sell ticket, num = %d\n", pShareRes->trainTicket);
        --pShareRes->trainTicket;
        ++cnt;
        //printf("after 1 sell ticket, num = %d\n", pShareRes->trainTicket);
        pthread_mutex_unlock(&pShareRes->mutex);
        // sleep(1);
    }
    printf("sellticket1 = %d\n", cnt);
}

void *sellTicket2(void *arg)
{
    int cnt = 0;
    shareRes_t *pShareRes = (shareRes_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&pShareRes->mutex);
        if (pShareRes->trainTicket <= 0)
        {
            // 在所有离开循环的场景，记得解锁
            pthread_mutex_unlock(&pShareRes->mutex);
            break;
        }
        //printf("before 2 sell ticket, num = %d\n", pShareRes->trainTicket);
        --pShareRes->trainTicket;
        ++cnt;
        //printf("after 2 sell ticket, num = %d\n", pShareRes->trainTicket);
        pthread_mutex_unlock(&pShareRes->mutex);
        // sleep(1);
    }
    printf("sellticket2 = %d\n", cnt);
}

int main()
{
    shareRes_t shareRes;
    shareRes.trainTicket = 20000000;

    pthread_mutex_init(&shareRes.mutex, NULL);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, sellTicket1, (void *)&shareRes);
    pthread_create(&tid2, NULL, sellTicket2, (void *)&shareRes);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&shareRes.mutex);
}