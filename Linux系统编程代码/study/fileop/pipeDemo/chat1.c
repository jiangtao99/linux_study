#include <myhead.h>

int main(int argc, char *argv[])
{
    // ./chat1 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");

    puts("pipe open");

}