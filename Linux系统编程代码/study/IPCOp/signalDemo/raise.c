#include <myhead.h>
void sigFunc(int num)
{
    printf("num = %d\n", num);
}

int main()
{
    signal(SIGINT, sigFunc);
    while(1)
    {
        getchar();
        raise(2);
    }

}