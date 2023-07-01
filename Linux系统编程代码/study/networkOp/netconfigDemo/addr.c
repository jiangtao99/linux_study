#include <myhead.h>
int main(int argc, char *argv[])
{
    //./addr 192.168.14.9 1234
    ARGS_CHECK(argc, 3);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;  //表示使用的是IPV4地址
    addr.sin_port = htons(atoi(argv[2]));   //先字符串转换成数字，再小端转大端
    addr.sin_addr.s_addr = inet_addr((argv[1]));
    printf("port = %x\n", addr.sin_port);
    printf("ip = %08x\n", addr.sin_addr.s_addr);
}