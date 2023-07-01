#include <myhead.h>
int main()
{
    //把代码段和pc指针画出来
    int i = 0;
    for(i = 0; i < 2; i++)
    {
        fork();
        // printf("-\n");
        printf("-");
    }
    return 0;
}