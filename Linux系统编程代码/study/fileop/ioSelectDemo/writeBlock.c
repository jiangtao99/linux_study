#include <myhead.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDWR);
    int fdw = open(argv[1], O_RDWR);

    printf("pipe open\n");
    char buf[4096];
    int cnt = 0;

    while(1)
    {
        printf("cnt = %d\n", cnt++);
        write(fdw, buf, sizeof(buf));
    }
    

    return 0;
}