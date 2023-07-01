#include <myhead.h>
// 确保hello在world之前打印
typedef struct shareRes_s
{
    int flag;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} shareRes_t;

void *threadFunc(void *arg)
{
    shareRes_t *pShareRes = (shareRes_t *)arg;
    // 使用条件变量要先加锁
    pthread_mutex_lock(&pShareRes->mutex);
    //flag是条件变量能否等待的一个很重要的状态
    if (pShareRes->flag == 0)
    {
        // 只有在加锁的状态下，才能使用wait
        puts("use");
        pthread_cond_wait(&pShareRes->cond, &pShareRes->mutex);
    }
    // 从wait中唤醒的话，说明前面的事件已经完成了
    puts("world");
    // 记得解锁
    pthread_mutex_unlock(&pShareRes->mutex);

    pthread_exit(NULL);
}

int main()
{
    shareRes_t shareRes;
    pthread_t tid;
    shareRes.flag = 0;
    // 1.初始化锁和条件变量
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    // 2.创建一个子线程
    pthread_create(&tid, NULL, threadFunc, (void *)&shareRes);
    // 3.先执行一个事件，然后唤醒等待在条件变量上的某个线程
    sleep(1);
    pthread_mutex_lock(&shareRes.mutex);
    puts("Hello");
    shareRes.flag = 1;
    pthread_cond_signal(&shareRes.cond);
    sleep(1);
    pthread_mutex_unlock(&shareRes.mutex);
    // 4.等待子线程的终止
    pthread_join(tid, NULL);

    // 5.销毁锁和条件变量
    pthread_cond_destroy(&shareRes.cond);
    pthread_mutex_destroy(&shareRes.mutex);
}