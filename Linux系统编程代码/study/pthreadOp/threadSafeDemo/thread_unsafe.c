#include <myhead.h>
void *threadFunc(void *arg)
{
    time_t now = time(NULL);
    char *p = ctime(&now);
    printf("child p = %s\n", p);
    sleep(5);
    printf("child p = %s\n", p);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    
    sleep(2);
    time_t mainNow = time(NULL);
    char *p = ctime(&mainNow);
    printf("main p = %s\n", p);
    

    pthread_join(tid, NULL);

}