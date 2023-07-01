#include "threadPool.h"

void cleanFunc(void *arg)
{
    threadPool_t *pThreadPool = (threadPool_t *)arg;
    pthread_mutex_unlock(&pThreadPool->taskQueue.mutex);
}

void *handleEvent(void *arg)
{
    threadPool_t *pThreadPool = (threadPool_t *)arg;
    int netfd;
    while(1)
    {
        printf("I am free, tid =  %lu\n", pthread_self());
        pthread_mutex_lock(&pThreadPool->taskQueue.mutex);
        pthread_cleanup_push(cleanFunc, (void *)pThreadPool);
        while(pThreadPool->taskQueue.size == 0)
        {
            pthread_cond_wait(&pThreadPool->taskQueue.cond, &pThreadPool->taskQueue.mutex);
        }
        //现在子线程苏醒了
        netfd = pThreadPool->taskQueue.pFront->netfd;   //拿到了队首的文件描述符
        taskDeQueue(&pThreadPool->taskQueue);
        
        pthread_cleanup_pop(1);
        //pthread_mutex_unlock(&pThreadPool->taskQueue.mutex);
        printf("I am working\n");
        transFile(netfd);
        printf("done\n");
        close(netfd); 
    }  
}

int makeWorker(threadPool_t *pThreadPool)
{
    for(int i = 0; i < pThreadPool->threadNum; ++i)
    {
        pthread_create(&pThreadPool->tid[i], NULL, handleEvent, (void *)pThreadPool);
    }
}