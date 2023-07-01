#include <myhead.h>
//在数据段
int global = 1;
int main()
{
    int stack = 2;
    int *pHeap = malloc(sizeof(int));
    *pHeap = 3;
    
    if(fork() == 0)
    {
        printf("I am child process. global = %d, stack = %d, *pHead = %d\n",
            global, stack, *pHeap);

        ++global;
        ++stack;
        ++*pHeap;
        printf("I am child process. global = %d, stack = %d, *pHead = %d\n",
            global, stack, *pHeap);
    }
    else
    {
        sleep(2);
        printf("I am parent process. global = %d, stack = %d, *pHead = %d\n",
            global, stack, *pHeap);
    }

    return 0;
}