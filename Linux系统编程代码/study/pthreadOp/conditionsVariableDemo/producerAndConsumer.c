#include <myhead.h>
#define NUM 10
typedef struct
{
    int elem[NUM];
    int size;
    int front; // 队首下标
    int rear;  // 下一个要入队的元素的下标
} queue_t;

// 每次用于入队和出队操作
int cnt = 0;

typedef struct
{
    queue_t queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shareRes_t;

void *producer(void *arg)
{
    shareRes_t *pShareRes = (shareRes_t *)arg;
    sleep(1);
    while (1)
    {
        pthread_mutex_lock(&pShareRes->mutex);
        if (pShareRes->queue.size == NUM)
        {
            pthread_cond_wait(&pShareRes->cond, &pShareRes->mutex);
        }
        pShareRes->queue.elem[pShareRes->queue.rear] = cnt++;
        pShareRes->queue.rear = (pShareRes->queue.rear + 1) % NUM;
        ++pShareRes->queue.size;
        printf("producer size = %d, front = %d, rear = %d\n",
               pShareRes->queue.size,
               pShareRes->queue.elem[pShareRes->queue.front],
               pShareRes->queue.elem[(pShareRes->queue.rear - 1 + NUM) % NUM]);
        // 如果加1以后表示产品队列里面有产品了，就通知消费者去消费
        if (pShareRes->queue.size == 1)
        {
            pthread_cond_signal(&pShareRes->cond);
        }
        pthread_mutex_unlock(&pShareRes->mutex);
        usleep(500000);
        
    }
}

void *consumer(void *arg)
{
    shareRes_t *pShareRes = (shareRes_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&pShareRes->mutex);
        if (pShareRes->queue.size == 0)
        {
            pthread_cond_wait(&pShareRes->cond, &pShareRes->mutex);
        }
        pShareRes->queue.front = (pShareRes->queue.front + 1) % NUM;
        --pShareRes->queue.size;
        printf("consumer size = %d, front = %d, rear = %d\n",
               pShareRes->queue.size,
               pShareRes->queue.elem[pShareRes->queue.front],
               pShareRes->queue.elem[(pShareRes->queue.rear - 1 + NUM) % NUM]);
        // 如果加1以后表示产品队列里面有产品了，就通知消费者去消费
        if (pShareRes->queue.size == 9)
        {
            pthread_cond_signal(&pShareRes->cond);
        }
        pthread_mutex_unlock(&pShareRes->mutex);
        usleep(800000);
    }
}

int main()
{
    shareRes_t shareRes;
    pthread_t tid1, tid2, tid3, tid4, tid5;
    bzero(&shareRes.queue, sizeof(shareRes.queue));

    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);

    pthread_create(&tid1, NULL, producer, (void *)&shareRes);
    pthread_create(&tid4, NULL, producer, (void *)&shareRes);
    pthread_create(&tid2, NULL, consumer, (void *)&shareRes);
    pthread_create(&tid3, NULL, consumer, (void *)&shareRes);
    pthread_create(&tid5, NULL, consumer, (void *)&shareRes);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);
}