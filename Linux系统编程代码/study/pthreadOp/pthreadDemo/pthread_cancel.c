#include <myhead.h>

void *threadFunc(void *arg)
{
    //1.线程取消成功的例子
    //printf是一个取消点
    //子线程会等printf调用完了之后线程才终止
    //printf("I am child thread, tid = %lu\n", pthread_self());
    //printf("no print\n");
    
    //2.线程取消失败的例子
    while(1)
    {
        pthread_testcancel();
    }
    
    pthread_exit((void *)0);
}

int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    THREAD_ERROR_CHECK(ret, "ptrhead_create");

    pthread_cancel(tid);
    void *retval;

    //主线程一直处于阻塞状态，直到tid线程终止
    //如果tid是被cancel掉的，retval就是-1
    pthread_join(tid, &retval);
    printf("child thread return value = %ld\n", (long)retval);
}