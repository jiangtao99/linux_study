#include <myhead.h>

int main(int argc, char *argv[])
{
    // ./chat2 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");
    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    puts("pipe open");

}