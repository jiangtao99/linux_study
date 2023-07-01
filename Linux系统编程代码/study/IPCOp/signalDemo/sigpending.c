#include<myhead.h>
void sigFunc(int num)
{
    printf("num = %d\n", num);
}

int main()
{
    signal(SIGQUIT, sigFunc);
    
    sigset_t set;
    sigemptyset(&set);

    sigaddset(&set, SIGQUIT);
    sigset_t oldset;
    
    //把3号信号加入屏蔽，把原来的屏蔽状况保存到oldset里
    sigprocmask(SIG_SETMASK, &set, &oldset);
    
    
    printf("I am going to sleep!\n");
    sleep(10);
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending, SIGQUIT)) 
    {
        printf("SIGQUIT is pending!\n");
    }
    else 
    {
        printf("SIGQUIT is not pending!\n");
    }
    
    printf("I wake up\n");
    
    //把3号信号解除阻塞
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    
    printf("byebye\n");
    return 0;
}