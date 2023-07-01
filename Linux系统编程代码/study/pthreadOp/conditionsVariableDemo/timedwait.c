#include <myhead.h>


int main()
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    struct timespec abstime;

    //获取现在的绝对时间
    time_t now = time(NULL);
    puts(ctime(&now));

    abstime.tv_sec = now + 3;
    abstime.tv_nsec = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_mutex_lock(&mutex);
    //pthread_cond_wait(&cond, &mutex);
    
    pthread_cond_timedwait(&cond, &mutex, &abstime);
    now = time(NULL);
    puts(ctime(&now));

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}