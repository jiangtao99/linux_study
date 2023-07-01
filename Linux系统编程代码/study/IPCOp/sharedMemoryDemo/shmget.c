#include <myhead.h>
int main()
{
    key_t key = ftok("file1", 1);
    ERROR_CHECK(key, -1, "ftok");
    printf("key = %d\n", key);
    int shmid = shmget(key, 4096, IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
}