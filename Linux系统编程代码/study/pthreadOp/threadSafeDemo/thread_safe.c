#include <myhead.h>
void *threadFunc(void *arg)
{
    time_t now = time(NULL);
    char buf[1024] = {0};

    //char *p = ctime_(&now);
    char *p = ctime_r(&now, buf);
    
    printf("child p = %s\n", p);
    sleep(5);
    printf("child p = %s\n", p);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);

    char buf[1024] = {0};

    sleep(2);
    time_t mainNow = time(NULL);
    //char *p = ctime(&mainNow);
    char *p = ctime_r(&mainNow, buf);
    
    printf("main p = %s\n", p);
    

    pthread_join(tid, NULL);

}