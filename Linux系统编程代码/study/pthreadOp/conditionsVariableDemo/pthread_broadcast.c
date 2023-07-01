#include <myhead.h>
// 确保hello在world之前打印
typedef struct shareRes_s
{
    int cnt;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} shareRes_t;

void *threadFunc(void *arg)
{
    shareRes_t *pShareRes = (shareRes_t *)arg;
    // 使用条件变量要先加锁
    pthread_mutex_lock(&pShareRes->mutex);
    //flag是条件变量能否等待的一个很重要的状态
    while (pShareRes->cnt == 0)
    {
        // 只有在加锁的状态下，才能使用wait
        pthread_cond_wait(&pShareRes->cond, &pShareRes->mutex);
    }
    printf("cnt = %d\n", pShareRes->cnt);
    pShareRes->cnt -= 1;
    printf("cnt = %d\n", pShareRes->cnt);
    // 从wait中唤醒的话，说明前面的事件已经完成了
    // 记得解锁
    pthread_mutex_unlock(&pShareRes->mutex);

    pthread_exit(NULL);
}

int main()
{
    shareRes_t shareRes;
    pthread_t tid1, tid2;
    shareRes.cnt = 0;
    // 1.初始化锁和条件变量
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    // 2.创建一个子线程
    pthread_create(&tid1, NULL, threadFunc, (void *)&shareRes);
    pthread_create(&tid2, NULL, threadFunc, (void *)&shareRes);

    // 3.先执行一个事件，然后唤醒等待在条件变量上的某个线程
    sleep(1);
    pthread_mutex_lock(&shareRes.mutex);
    shareRes.cnt = 1;
    //pthread_cond_signal(&shareRes.cond);
    pthread_cond_broadcast(&shareRes.cond);
    pthread_mutex_unlock(&shareRes.mutex);
    
    // 4.等待子线程的终止
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 5.销毁锁和条件变量
    pthread_cond_destroy(&shareRes.cond);
    pthread_mutex_destroy(&shareRes.mutex);
}