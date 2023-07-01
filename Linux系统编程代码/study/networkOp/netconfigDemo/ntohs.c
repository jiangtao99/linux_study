#include <myhead.h>

//主机显示的是小端
//写到网络时要转换成大端
int main()
{
    unsigned short s = 0x1234;
    printf("s = %x\n", s);
    printf("htons(s) = %x\n", htons(s));

    unsigned int i = 0x12345678;
    printf("i = %x\n", i);
    printf("htonl(i) = %x\n", htonl(i));

}