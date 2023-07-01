#include <myhead.h>

int main(int argc, char *argv[])
{
    //char buf[1024] = {0};
    //char *ret = getcwd(buf, sizeof(buf));
    
    //这里用栈的坏处是，并不知道内容有多长。所以建议可以多分配点
    //或者写NULL，让被调函数来分配，返回值来接收地址，注意这里是在堆中分配的，要注意手动回收
    //不free会造成内存泄漏
    char *ret = getcwd(NULL, 0);

    ERROR_CHECK(ret, NULL, "getcwd");

    printf("ret = %p, ret = %s\n", ret, ret);
    //printf("buf = %p, buf = %s\n", buf, buf);
    free(ret);

    return 0;
}