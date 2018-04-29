#ifndef OS_H
#define OS_H

#include <unistd.h>
extern char ** environ;

class OS
{
public:
    OS(char ** argv);
    ~OS();

    // 设置进程名称
    void SetProcessTitle(const char * title);

    // 设置进程为守护进程模式
    void daemon(void);

    void * getSharedMem(void);

    // 设置最大连接数
    int SetMaxConn(int maxconn);
private:
    char * environ_last;
    char ** os_argv;
    void * shared_mem;
    OS(const OS & o);
    OS & operator=(const OS & o);
};
#endif // OS_H
