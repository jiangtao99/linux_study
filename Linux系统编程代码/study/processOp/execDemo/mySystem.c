#include <myhead.h>

//作业：写一个mySystem程序，实现system函数类似的功能（只需要创建一个子进程）。

void mySystem(char * command);

int main()
{
    //system("./add 3 4");
    mySystem("./add 3 4");
}

void mySystem(char *command)
{
    char buf[1024];
    char *word;
    char *argvec[10] = {0}; 
    int i = 1;
    strcpy(buf, command);
    word = strtok(buf, " ");
    argvec[0] = word;
    puts(word);
    while((word = strtok(NULL, " ")) != NULL)
    {
        argvec[i] = word;
        i++;
    }
    if(fork() == 0)
    {
        execv(argvec[0], argvec);
    }   
    else{
        wait(NULL);
    }
}