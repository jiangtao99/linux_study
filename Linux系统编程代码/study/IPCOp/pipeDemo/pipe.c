#include <myhead.h>
int main()
{
    //一个进程就是自言自语
    int pipefds[2];
    pipe(pipefds);
    printf("pipefd[0] = %d, pipefd[1] = %d\n", pipefds[0], pipefds[1]);
    //[1]写
    write(pipefds[1], "jiayou", 6);
    char buf[10] = {0};
    //[0]读
    read(pipefds[0], buf, sizeof(buf));
    printf("msg from pipe = %s\n", buf);
}