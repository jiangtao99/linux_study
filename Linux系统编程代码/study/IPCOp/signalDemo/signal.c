#include <myhead.h>

void sigFunc(int num)
{
    printf("num = %d\n", num);
    //当信号到来时，调用此函数如果printf没有加\n，则数据依旧在用户态缓冲区中，没有被冲到屏幕上面
    //记得加换行符
    //printf("num = %d", num);
    printf("before sleep, I am %d\n", num);
    sleep(3);
    printf("after sleep, I am %d\n", num);
}

int main()
{
    void (*ret)(int);
    ret = signal(SIGINT, sigFunc);
    ERROR_CHECK(ret, SIG_ERR, "signal");

    //ret = signal(SIGQUIT, sigFunc);
    //ERROR_CHECK(ret, SIG_ERR, "signal");
    while(1)
    {

    }
}