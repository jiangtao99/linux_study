#include <myhead.h>
typedef struct msgbuf
{
    long mtype;
    char mtext[256];
}myMsg_t;

int main()
{
    int msgid = msgget(1000, IPC_CREAT|0600);
    ERROR_CHECK(msgid, -1, "msgget");
    long type;
    printf("who are you? 1.huangxiaoming 2.wuyifan 3.caixukun\n");
    scanf("%ld", &type);
    myMsg_t msg;
    memset(&msg, 0, sizeof(msg));
    
    //msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0);
    int ret = msgrcv(msgid, &msg, sizeof(msg.mtext), 0, IPC_NOWAIT);
    ERROR_CHECK(ret, -1, "msgrcv");

    printf("you are %ld, msg = %s\n", type, msg.mtext);
}