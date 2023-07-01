#include <myhead.h>

int main(int argc, char *argv[])
{
    // ./cp src desc
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    ERROR_CHECK(fdw, -1, "open fdw");

    char buf[4096] = {0};
    ssize_t ret = 0;

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        ret = read(fdr, buf, sizeof(buf));
        if(ret == 0)
        {
            break;
        }

        //读多少写多少
        write(fdw, buf, ret);        
    }    

    close(fdr);
    close(fdw);

    return 0;
}