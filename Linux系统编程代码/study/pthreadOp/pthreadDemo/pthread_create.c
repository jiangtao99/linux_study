#include <myhead.h>

void *threadFunc(void *arg)
{
    printf("I am child thread, tid = %lu\n", pthread_self());
}

int main()
{
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    //sleep(1);
    usleep(20);
    //return 0;
    _exit(0);
}