
#include <myhead.h>

typedef struct task_s
{
    int netfd;  //传递文件描述符
    struct task_s *pNext;
} task_t;

typedef struct taskQueue_s
{
    task_t *pFront; //队首指针
    task_t *pRear;  //队尾指针
    int size;   //队列现在的长度
    //有同步要求，所以用条件变量
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} taskQueue_t;

typedef struct threadPool_s
{  
    pthread_t *tid; //子线程的数组
    int threadNum;  //子线程的数量
    taskQueue_t taskQueue;
    int exitFlag;
} threadPool_t;

typedef struct train_s
{
    /* data */
    int length;
    char buf[1000];
} train_t;
int taskEnQueue(taskQueue_t *pTaskQueue, int netfd);
int taskDeQueue(taskQueue_t *pTaskQueue);
int threadPoolInit(threadPool_t *pThreadPool, int workerNum);
int makeWorker(threadPool_t *pThreadPool);
int tcpInit(int *pSockfd, const char *ip, const char *port);
int epollAdd(int fd, int epfd);
int epollDel(int fd, int epfd);
int transFile(int netfd);