#include <myhead.h>
// 多线程共享堆空间
// 主线程和子线程使用同一个数值的地址，怎么传递这个地址呢？使用参数传递
//发现我们多线程只共享一个4字节大小的数据，但是传递参数指针却用了8个字节的大小，这样有必要吗


void *threadFunc(void *arg)
{
    printf("I am child thread, tid = %lu\n", pthread_self());
    int *pHeap = (int *)arg;
    *pHeap = 100;

    printf("child thread, *pHeap = %d\n", *pHeap);

}

//演示pthread系列函数的报错信息，以及能创建多少子线程
int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int *pHeap = malloc(sizeof(int));
    *pHeap = 0;

    int ret = pthread_create(&tid, NULL, threadFunc, (void *)pHeap);
    THREAD_ERROR_CHECK(ret, "ptrhead_create");

    //sleep(1);
    printf("main thread, *pHeap = %d\n", *pHeap);

    sleep(1);
}