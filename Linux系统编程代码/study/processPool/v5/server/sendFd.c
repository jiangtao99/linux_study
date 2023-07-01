#include <myhead.h>

//发送一个文件描述符
int sendFd(int pipeFd, int fdToSend)
{
    struct msghdr hdr;
    bzero(&hdr, sizeof(hdr));
    //第一个部分发文本数据
    struct iovec iov[1]; //长度为1时离散数据退化成连续数据
    char buf[10] = "hello";
    iov[0].iov_base = buf;
    iov[0].iov_len = 5;
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;

    //第二个部分发控制数据
    struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1, CMSG_LEN(sizeof(int))); 
    pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int)); //要承载的控制信息是一个整型数据
    pcmsghdr->cmsg_level = SOL_SOCKET;
    pcmsghdr->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(pcmsghdr) = fdToSend;
    hdr.msg_control = pcmsghdr;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = sendmsg(pipeFd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    return 0;
}

//接收一个文件描述符
int recvFd(int pipeFd, int *pFdToRecv)
{
    struct msghdr hdr;
    bzero(&hdr, sizeof(hdr));
    //第一个部分发文本数据
    struct iovec iov[1]; //长度为1时离散数据退化成连续数据
    char buf[10] = {0};
    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);   //这里无论什么情况不要写0
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1; 

    //第二个部分发控制数据
    struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1, CMSG_LEN(sizeof(int))); 
    pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int)); //要承载的控制信息是一个整型数据
    pcmsghdr->cmsg_level = SOL_SOCKET;
    pcmsghdr->cmsg_type = SCM_RIGHTS;
    hdr.msg_control = pcmsghdr;
    hdr.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = recvmsg(pipeFd, &hdr, 0);
    ERROR_CHECK(ret, -1, "recvmsg");
    *pFdToRecv = *(int *)CMSG_DATA(pcmsghdr);
    return 0;
}

