#include <myhead.h>

void sigFunc(int num)
{

}

void mySleep(int second)
{
    signal(SIGALRM, sigFunc);
    sigset_t set;
    sigset_t oldset;

    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    //阻塞alarm信号
    sigprocmask(SIG_SETMASK, &set, NULL);
    printf("block!\n");

    alarm(second);
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    pause();
}

int main()
{
    mySleep(5);
}