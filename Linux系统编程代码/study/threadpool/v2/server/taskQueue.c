#include "threadPool.h"

int taskEnQueue(taskQueue_t *pTaskQueue, int netfd)
{
    //如果在栈上，函数调用完内存就释放了，所以要用堆
    task_t *pTask = (task_t *)calloc(1, sizeof(task_t));
    pTask->netfd = netfd;
    if(pTaskQueue->size == 0)
    {
        pTaskQueue->pFront = pTask;
        pTaskQueue->pRear = pTask;
    }
    else
    {
        pTaskQueue->pRear->pNext = pTask;
        pTaskQueue->pRear = pTask;
    }
    ++pTaskQueue->size;
    return 0;
}

int taskDeQueue(taskQueue_t *pTaskQueue)
{
    task_t *pCur = pTaskQueue->pFront;
    if(pCur == NULL)
    {
        return -1;
    }
    pTaskQueue->pFront = pCur->pNext;
    free(pCur);
    --pTaskQueue->size;
    return 0;
}