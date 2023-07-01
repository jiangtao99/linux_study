#include <myhead.h>
#include "threadPool.h"
int transFile(int netfd)
{
    int fd = -1;
    int ret = -1;
    char buf[4096] = {0};

    train_t train;
    //1.打开要传输的文件
    fd = open("file1", O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    
    //2.将文件名的长度记录进要发送的小火车头中
    train.length = 5;
    //3.将文件名的内容记录进要发送的小火车车厢中
    strcpy(train.buf, "file1");
    //4.发送文件名（发送小火车）
    ret = send(netfd, &train, sizeof(train.length) + train.length, MSG_NOSIGNAL);
    ERROR_CHECK(ret, -1, "send");


    //5.获取文件大小（5~8是将文件的大小传给客户端）
    struct stat statbuf;
    ret = fstat(fd, &statbuf);
    ERROR_CHECK(ret, -1, "fstat");
    //6.将文件大小的长度所占的字节数保存在火车头中
    train.length = 4;
    //7.将文件的大小保存在火车车厢中
    int fileSize = statbuf.st_size; //长度转换成int
    memcpy(train.buf, &fileSize, sizeof(int));//int存入小火车
    //8.发送文件的大小
    send(netfd, &train, train.length + sizeof(train.length), MSG_NOSIGNAL);


    //9.发送文件内容
    sendfile(netfd, fd, NULL, fileSize);

    train.length = 0;
    ret = send(netfd, &train, sizeof(train.length) + train.length, MSG_NOSIGNAL);
    ERROR_CHECK(ret, -1, "send");

    close(fd);
}