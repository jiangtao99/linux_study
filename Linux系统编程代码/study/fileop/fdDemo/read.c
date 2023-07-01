#include <myhead.h>
int main(int argc, char *argv[]) 
{
    // ./open file1
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    //读二进制文件
    /* int buf;
    read(fd, &buf, sizeof(buf));
    printf("buf = %d\n", buf);
     */

    //读文本文件
    char buf[10] = {0};
    ssize_t ret = read(fd, buf, sizeof(buf));
    puts(buf);
    close(fd);
    



    return 0;
}