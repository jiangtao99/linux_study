#include <myhead.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");
    puts("pipe open!");

    write(fdw, "hello", 5);

    close(fdw);

    return 0;
}