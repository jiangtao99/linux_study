#include <myhead.h>
// 多线程共享数据端
// 两个线程之间数据传递非常容易，代价是很容易发生竞争条件
int global = 10;

void *threadFunc(void *arg)
{
    printf("I am child thread, tid = %lu\n", pthread_self());

    printf("child thread, global = %d\n", global);
}

//演示pthread系列函数的报错信息，以及能创建多少子线程
int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "ptrhead_create");

    sleep(1);
    global = 200;
    printf("main thread, global = %d\n", global);

    sleep(1);
}