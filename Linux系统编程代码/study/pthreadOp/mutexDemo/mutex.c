#include <myhead.h>
#define NUM 10000000
typedef struct shareRes_s 
{
    pthread_mutex_t mutex;
    int count;
} shareRes_t;

void *threadFunc(void *arg)
{
    shareRes_t *pShareRes = (shareRes_t *)arg;
    for(int i = 0; i < NUM; ++i)
    {
        pthread_mutex_lock(&pShareRes->mutex);
        ++pShareRes->count;
        pthread_mutex_unlock(&pShareRes->mutex);
    }
}

int main()
{
    //静态初始化
    //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
    //pthread_mutex_t mutex;
    //如果第二个参数是NULL，表示以默认形式初始化锁
    //pthread_mutex_init(&mutex, NULL); 
    //pthread_mutex_destroy(&mutex);

    shareRes_t shareRes;
    shareRes.count = 0;
    pthread_mutex_init(&shareRes.mutex, NULL);

    pthread_t tid;
    struct timeval timeBeg, timeEnd;

    gettimeofday(&timeBeg, NULL);
    int ret = pthread_create(&tid, NULL, threadFunc, (void *)&shareRes);
    THREAD_ERROR_CHECK(ret, "pthread_create");

    for(int i = 0; i < NUM; ++i)
    {
        pthread_mutex_lock(&shareRes.mutex);        
        ++shareRes.count;
        pthread_mutex_unlock(&shareRes.mutex);
    }

    pthread_join(tid, NULL);
    gettimeofday(&timeEnd, NULL);

    printf("total time = %ld us\n", 1000000 * (timeEnd.tv_sec - timeBeg.tv_sec) + (timeEnd.tv_usec - timeBeg.tv_usec));
    printf("count = %d\n", shareRes.count);
    pthread_mutex_destroy(&shareRes.mutex);
}