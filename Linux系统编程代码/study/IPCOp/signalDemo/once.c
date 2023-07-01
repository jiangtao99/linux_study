#include <myhead.h>

void sigFunc(int num)
{
    printf("num = %d\n", num);
    //给2信号重新注册一个新的行为，默认行为
    signal(SIGINT, SIG_DFL);
}

int main()
{
    void (*ret)(int);
    ret = signal(SIGINT, sigFunc);
    ERROR_CHECK(ret, SIG_ERR, "signal");
    
    char buf[100] = {0};
    read(STDIN_FILENO, buf, sizeof(buf));
    printf("buf = %s\n", buf);
}