#include <myhead.h>

int main()
{
    pthread_mutex_t mutex;
    
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    //普通锁和默认是一样的
    //pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);

    //检错锁，对同一把锁加锁，第二次加锁会报错
    //pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);

    //递归锁（可重入锁），对同一把锁加锁，什么都不会报错
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
    
    pthread_mutex_init(&mutex, &mutexattr);
    
    //第一次加锁
    int ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_lock1");
    

    //第二次加锁
    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_lock2");
    
    pthread_mutexattr_destroy(&mutexattr);
    pthread_mutex_destroy(&mutex);
}