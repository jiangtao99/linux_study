#include <myhead.h>
void sigFunc(int signum)
{
    time_t now = time(NULL);
    printf("%s\n", ctime(&now));
}

int main()
{
    signal(SIGALRM, sigFunc);
    sigFunc(0);
    alarm(3);
    pause();
}