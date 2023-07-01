#include <myhead.h>

void sigFunc(int signum)
{
    time_t now = time(NULL);
    puts(ctime(&now));
}

int main()
{
    struct itimerval itimer;
    itimer.it_value.tv_sec = 5;
    itimer.it_value.tv_usec = 0;
    itimer.it_interval.tv_sec = 1;
    itimer.it_interval.tv_usec = 0;
    
    //signal(SIGALRM, sigFunc);
    //setitimer(ITIMER_REAL, &itimer, NULL);
    
    signal(SIGVTALRM, sigFunc);
    setitimer(ITIMER_VIRTUAL, &itimer, NULL);

    signal(SIGPROF, sigFunc);
    setitimer(ITIMER_PROF, &itimer, NULL);

    sigFunc(0);
    while(1);
}