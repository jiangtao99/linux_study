#include <myhead.h>
int main()
{
    //意思是建立了管道，从./print中读到管道fp中
    FILE *fp = popen("./print", "r");
    ERROR_CHECK(fp, NULL, "popen");
    char buf[1024] = {0};
    fread(buf, 1, sizeof(buf), fp);
    printf("msg from pipe, buf = %s\n", buf);   
}