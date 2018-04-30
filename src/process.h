#ifndef Process_H
#define Process_H

#include <unistd.h>
extern char ** environ;

#include <boost/noncopyable.hpp>

class Process : boost::noncopyable
{
public:
    Process(char ** argv);
    ~Process();

    // 设置进程名称
    void SetProcessTitle(const char * title);

    // 设置进程为守护进程模式
    void daemon(void);

    void * getSharedMem(void);

    // 设置最大连接数
    int SetMaxConn(int maxconn);

    //绑定信号处理函数
    void SignalBind(void);
    static void signal_handle(int signo);
private:
    char * environ_last;
    char ** os_argv;
    void * shared_mem;
};
#endif // Process_H
