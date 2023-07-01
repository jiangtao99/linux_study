#include <myhead.h>
void func()
{
    //exit(123);
    _exit(123);
}

int main()
{
    printf("hello");
    func();
    return 100;
}