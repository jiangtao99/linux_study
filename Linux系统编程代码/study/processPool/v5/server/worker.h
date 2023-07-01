#include <myhead.h>
enum {
    FREE,
    BUSY
};
typedef struct workerData_s
{
    pid_t pid;
    int status;
    int pipeFd;
} workerData_t;


int eventHandler(int pipeFd);
int makeChild(workerData_t *workerArr,  int workerNum);