#include <myhead.h>
int main(int argc, char *argv[]) 
{
    // ./open file1
    ARGS_CHECK(argc, 2);

    //1.以只写的方式打开文件
    //int fd = open(argv[1], O_WRONLY);


    //2.open三参数
    //注：创建文件的行为总是会受到umask的影响
    //int fd = open(argv[1], O_WRONLY|O_CREAT, 0666);

    //3.O|EXCL，确保一定能创建一个新的文件
    int fd = open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0666);


    ERROR_CHECK(fd, -1, "open");
    printf("fd = %d\n", fd);

    close(fd);

    return 0;
}