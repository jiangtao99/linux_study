#include <myhead.h>

//fopen的a模式
int mode_a(int argc, char *argv[]) 
{
    // ./fopen file1
    ARGS_CHECK(argc, 2);
    FILE *fp = fopen(argv[1], "a");
    ERROR_CHECK(fp, NULL, "fopen");
    fwrite("howareyou", 1, 9, fp);
    fclose(fp);
}

//fopen的a+模式
int mode_aplus(int argc, char *argv[])
{
    // ./fopen file1
    ARGS_CHECK(argc, 2);
    FILE *fp = fopen(argv[1], "a+");
    ERROR_CHECK(fp, NULL, "fopen");

    char buf[10] = {0};
    printf("%s", buf);           
    fread(buf, 1, 9, fp);

    //ftell,查看ptr的位置
    printf("before fseek, loc = %ld\n", ftell(fp));
    //在写入前，手动将ptr移动到最前面
    fseek(fp, 0, SEEK_SET);
    printf("after fseek, loc = %ld\n", ftell(fp));
    
    fwrite("howareyou", 1, 9, fp);

    fclose(fp);
}

int main(int argc, char *argv[])
{
    mode_aplus(argc, argv);
    return 0;
}