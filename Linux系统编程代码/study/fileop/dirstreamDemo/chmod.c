#include <myhead.h>

int main(int argc, char *argv[]) {
    //typedef unsigned int mode_t;
    //mode_t t;
    // ./chmod 777 dir1
    ARGS_CHECK(argc, 3);

    mode_t mode;
    sscanf(argv[1], "%o", &mode);
    int ret = chmod(argv[2], mode);
    ERROR_CHECK(ret, -1, "chmod");
    
    return 0;
}