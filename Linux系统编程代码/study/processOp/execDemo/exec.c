#include <myhead.h>
int main()
{
    printf("before exec, pid = %d!\n", getpid());
    //以空指针结尾
    //execl("add", "./add", "3", "4", NULL);
    
    char *argvector[] = {"./add",  "3", "4", NULL};
    execv("add", argvector);


    printf("after exec!\n");
}