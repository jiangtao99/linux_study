#include <myhead.h>
int main()
{
    int cnt = 0;
    while(1)
    {
        printf("helloworld, cnt=%d\n", ++cnt);
        sleep(1);
    }
    return 0;
}