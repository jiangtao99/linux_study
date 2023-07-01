#include <myhead.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    FILE *fp = fopen(argv[1], "r+");
    ERROR_CHECK(fp, NULL, "fopen");

    printf("fileno = %d\n", fp->_fileno);
    printf("fileno = %d\n", fileno(fp));

    //这里玩个花活，这里用文件指针的形式打开文件，但是用文件描述符去操作文件
    int ret = write(fileno(fp), "hello", 5);
    ERROR_CHECK(ret, -1, "write");

    fclose(fp);
}