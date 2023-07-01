#include <myhead.h>

void clean1(void *arg)
{
    printf("I am clean1\n");
    free(arg);
}

void clean2(void *arg)
{
    printf("I am clean2\n");
    free(arg);
}

void *threadFunc(void *arg)
{
    printf("I am child thread, tid = %lu\n", pthread_self());
    int *p1 = malloc(4);
    //申请资源后马上将对应的清理行为压栈
    pthread_cleanup_push(clean1, p1);
    pthread_exit(NULL);
    int *p2 = malloc(4);
    pthread_cleanup_push(clean2, p2);

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);

}

//演示pthread系列函数的报错信息，以及能创建多少子线程
int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "ptrhead_create");

    pthread_join(tid, NULL);

}