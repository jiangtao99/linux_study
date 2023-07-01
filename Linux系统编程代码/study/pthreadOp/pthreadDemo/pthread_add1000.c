#include <myhead.h>
#define NUM 10000000
void *threadFunc(void *arg)
{
    int *pCount = (int *)arg;
    for(int i = 0; i < NUM; ++i)
    {
        ++*pCount;
    }
}

//演示pthread系列函数的报错信息，以及能创建多少子线程
int main()
{
    int count = 0;
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, threadFunc, (void *)&count);
    THREAD_ERROR_CHECK(ret, "ptrhead_create");

    for(int i = 0; i< NUM; ++i)
    {
        ++count;
    }

    pthread_join(tid, NULL);
    printf("count = %d\n", count);
}