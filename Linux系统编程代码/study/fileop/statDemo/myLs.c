#include <myhead.h>
/*
程序的问题：当在当前目录下使用./myLs时没有问题；当不在当前目录下使用./myLs时结果错误，原因是什么？
报错信息是stat:No such file or directory
即pdirent->d_name出错了，因为参数要的是文件的路径，而不是文件的名字，因为文件的路径和文件的名字不总是相同的，
只有这个文件在当前目录下，文件的名字和文件的路径才是相同的
既然提供的目录不在当前目录下，可以通过chdir过去
*/
//获取文件类型
void getFileType(const mode_t m, char *t);
//获取ugo三者权限
void getugo(const mode_t m, char *ugo);
//获取修改时间
void getMTime(const time_t *timep, char *timebuf);


int main(int argc, char *argv[])
{
    // ./myLs dir
    ARGS_CHECK(argc, 2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir");

    struct dirent *pdirent;
    struct stat statbuf;
    int ret;
    //文件类型
    char type;
    //用户权限
    char ugo[16] = {0};
    //修改时间
    char timebuf[64] = {0};
    // 将指定目录变为当前目录
    ret = chdir(argv[1]);
    ERROR_CHECK(ret, -1, "chdir");

    while ((pdirent = readdir(dirp)) != NULL)
    {
        memset(ugo, 0, sizeof(ugo));
        memset(timebuf, 0, sizeof(timebuf));
        // 文件名只有在当前目录下才是当前路径
        ret = stat(pdirent->d_name, &statbuf);
        ERROR_CHECK(ret, -1, "stat");

        getFileType(statbuf.st_mode, &type);

        getugo(statbuf.st_mode, ugo);

        getMTime(&statbuf.st_mtime, timebuf);

        printf("%c%s %ld %s %s %7ld %s %s\n",
               type, ugo, statbuf.st_nlink, getpwuid(statbuf.st_uid)->pw_name, getgrgid(statbuf.st_gid)->gr_name,
               statbuf.st_size, timebuf, pdirent->d_name);
    }

    closedir(dirp);

    return 0;
}

void getFileType(const mode_t m, char *t)
{
    switch (m & S_IFMT)
    {
    case S_IFBLK:
        *t = 'b';
        break;
    case S_IFCHR:
        *t = 'c';
        break;
    case S_IFDIR:
        *t = 'd';
        break;
    case S_IFIFO:
        *t = 'p';
        break;
    case S_IFLNK:
        *t = 'l';
        break;
    case S_IFREG:
        *t = '-';
        break;
    case S_IFSOCK:
        *t = 's';
        break;
    default:
        *t = '?';
        break;
    }
}

void getugo(const mode_t m, char *ugo) 
{
    char buf[16] = {0};
    sprintf(buf, "%o", m);
    int bufLen = strlen(buf);
    int cnt = 3;
    while(cnt > 0) 
    {
        switch(buf[bufLen - cnt])
        {
            case '0':
                strcat(ugo, "---");
                break;
            case '1':
                strcat(ugo, "--x");
                break;
            case '2':
                strcat(ugo, "-w-");
                break;
            case '3':
                strcat(ugo, "-wx");
                break;
            case '4':
                strcat(ugo, "r--");
                break;
            case '5':
                strcat(ugo, "r-x");
                break;
            case '6':
                strcat(ugo, "rw-");
                break;
            case '7':
                strcat(ugo, "rwx");
                break;
        }
        cnt--;
    } 
}

void getMTime(const time_t *timep, char *timebuf)
{
    struct tm *time;
    time = localtime(timep);
    char ca[8] = {0};
    char mday[8] = {0};
    switch(time->tm_mon)
    {
        case 0:
            sprintf(ca, "%s", "Jan  ");
            break;
        case 1:
            sprintf(ca, "%s", "Feb  ");
            break;
        case 2:
            sprintf(ca, "%s", "Mar  ");
            break;
        case 3:
            sprintf(ca, "%s", "Apr  ");
            break;
        case 4:
            sprintf(ca, "%s", "May  ");
            break;
        case 5:
            sprintf(ca, "%s", "Jun  ");
            break;
        case 6:
            sprintf(ca, "%s", "Jul  ");
            break;
        case 7:
            sprintf(ca, "%s", "Aug  ");
            break;
        case 8:
            sprintf(ca, "%s", "Sep  ");
            break;
        case 9:
            sprintf(ca, "%s", "Oct  ");
            break;
        case 10:
            sprintf(ca, "%s", "Nov  ");
            break;
        case 11:
            sprintf(ca, "%s", "Dec  ");
            break;
    }
    sprintf(timebuf, "%s%d%s%d%s%d", ca, time->tm_mday, " ", time->tm_hour, ":", time->tm_min); 
}