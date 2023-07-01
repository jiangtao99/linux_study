#include <myhead.h>

void *threadFunc(void *arg)
{
    printf("I am child thread, tid = %lu\n", pthread_self());
    //return (void *)12345;
    pthread_exit((void *)23456);
}

//演示pthread系列函数的报错信息，以及能创建多少子线程
int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "ptrhead_create");

    //long retval;    //申请了8个字节
    //主线程要把&retval所指的内存空间，存储子线程的返回值
    //pthread_join(tid, (void **)&retval);
    //printf("ret = %ld\n", retval);

    void *retval; //申请了8个字节
    pthread_join(tid, &retval);
    printf("retval = %ld\n", (long)retval);

}