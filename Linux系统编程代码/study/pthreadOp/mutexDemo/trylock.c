#include <myhead.h>

int main()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    //第一次加锁
    int ret = pthread_mutex_trylock(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_trylock1");
    
    puts("lock first time!");

    //第二次加锁
    ret = pthread_mutex_trylock(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_trylock2");
    
    puts("lock second time!");

    pthread_mutex_destroy(&mutex);
}