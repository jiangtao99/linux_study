#include <myhead.h>
int main()
{
    int lhs, rhs;
    scanf("%d%d", &lhs, &rhs);
    printf("I am child process. pid = %d, ppid = %d\n", getpid(), getppid());  
    printf("%d - %d = %d\n", lhs, rhs, lhs - rhs);
    sleep(20);
}