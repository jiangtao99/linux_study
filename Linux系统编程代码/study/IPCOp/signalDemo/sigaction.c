#include <myhead.h>

void sigFunc(int num)
{
    printf("num = %d\n", num);
    //给2信号重新注册一个新的行为，默认行为
}

int main()
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = sigFunc;
    act.sa_flags = SA_RESTART;
    int ret = sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret, -1, "sigaction");
    
    char buf[100] = {0};
    read(STDIN_FILENO, buf, sizeof(buf));
    printf("buf = %s\n", buf);
}