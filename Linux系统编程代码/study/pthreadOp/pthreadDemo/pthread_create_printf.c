#include <myhead.h>

void *threadFunc(void *arg)
{
    printf("I am child thread, tid = %lu\n", pthread_self());

}

//演示pthread系列函数的报错信息，以及能创建多少子线程
int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "ptrhead_create");
}