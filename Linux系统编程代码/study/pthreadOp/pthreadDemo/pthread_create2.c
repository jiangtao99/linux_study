#include <myhead.h>

void *threadFunc(void *arg)
{
    //printf("I am child thread, tid = %lu\n", pthread_self());
    while(1)
    {
        sleep(1);
    }
}

//演示pthread系列函数的报错信息，以及能创建多少子线程
int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    for(int i = 0;; ++i)
    {
        int ret = pthread_create(&tid, NULL, threadFunc, NULL);
        THREAD_ERROR_CHECK(ret, "pthread_create");
        if(ret != 0)
        {
            printf("i = %d\n", i);
            break;
        }
    }
}