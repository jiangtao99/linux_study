#include <myhead.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);

    int oldfd = open(argv[1], O_RDWR);
    int savefd = 5;
    ERROR_CHECK(oldfd, -1, "open");

    printf("You can see me!\n");

    dup2(STDOUT_FILENO, STDOUT_FILENO);
    printf("You can't see me!\n");


    return 0;
}