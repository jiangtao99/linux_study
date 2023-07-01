#include <myhead.h>
int main()
{
    FILE *fp = popen("./sub", "w");
    ERROR_CHECK(fp, NULL, "popen");
    printf("I am parent process. pid = %d, ppid = %d\n", getpid(), getppid());  
    fwrite("1000 999", 1, 8, fp);

    sleep(20);
}