#include <myhead.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    // argv[1] "-9"
    int sig = atoi(argv[1] + 1);
    pid_t pid = atoi(argv[2]);
    int ret = kill(pid, sig);
    ERROR_CHECK(ret, -1, "kill");
}