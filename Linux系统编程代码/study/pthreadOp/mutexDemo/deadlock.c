#include <myhead.h>

int main()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    //第一次加锁
    pthread_mutex_lock(&mutex);
    puts("lock first time!");
    pthread_mutex_lock(&mutex);
    puts("lock second time!");

    pthread_mutex_destroy(&mutex);
}