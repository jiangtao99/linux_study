#include <myhead.h>

void Daemon()
{
    if(fork() != 0)
    {
        exit(0);
    }    
    // 创建出来一个新的会话
    setsid();
    for(int i = 0; i < 2; ++i)
    {
        close(i);
    }
    chdir("/");
    umask(0);
}

int main()
{
    Daemon();
    for(int i = 0; i < 100; i++)
    {
        time_t now = time(NULL);
        struct tm *pTm = localtime(&now);
        syslog(LOG_INFO, "%4d%2d%2d %2d:%2d:%2d\n", 
            pTm->tm_year+1900,pTm->tm_mon+1, pTm->tm_mday,
            pTm->tm_hour,pTm->tm_min, pTm->tm_sec);
        sleep(2);
    }
}