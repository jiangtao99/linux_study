#include <myhead.h>
int main(int argc, char *argv[]) 
{
    // ./open file1
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    char buf[10] = "world";
    write(fd, buf, strlen(buf));


    close(fd);
    
    return 0;
}