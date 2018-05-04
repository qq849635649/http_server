#include "process.h"

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <execinfo.h>

#include <stdexcept>
using namespace std;

#include "log/logger.h"
#include "mainloop.h"

#define SHARED_KEY 65433

struct signal_t
{
    int signo;
    void (*handle)(int sig);
};

static struct signal_t signalSets[] =
{
    {SIGSYS,	SIG_IGN},
    {SIGPIPE,	SIG_IGN},
    {SIGHUP,	SIG_IGN},
    {SIGCHLD,	Process::signal_handle},
    {SIGSEGV,	Process::signal_handle},
    {SIGILL,	Process::signal_handle},
    {SIGBUS,	Process::signal_handle},
    {SIGFPE,	Process::signal_handle},
    {SIGINT,	Process::signal_handle},
    {SIGQUIT,	Process::signal_handle},
    {SIGKILL,	Process::signal_handle},
    {SIGTERM,	Process::signal_handle},
    {SIGSTOP,	Process::signal_handle},
};

Process::Process(char ** argv)
{
    int i;
    size_t size = 0;
    os_argv = argv;
    environ_last = argv[0];

    for(i = 0; environ[i]; i++)
        size += strlen(environ[i]) + 1;

    char * p = new char[size];

    for(i = 0; argv[i]; i++)
    {
        if(environ_last == argv[i])
            environ_last = argv[i] +  strlen(argv[i]) + 1;
    }

    for(i = 0; environ[i]; i++)
    {
        if(environ_last == environ[i])
        {
            size = strlen(environ[i]) + 1;
            environ_last = environ[i] + size;
            strncpy(p, environ[i], size);
            environ[i] = p;
            p += size;
        }
    }

    environ_last--;

    size = 4096;

    int shmid = shmget(SHARED_KEY, size, IPC_CREAT|0666);
    if(shmid < 0)
        throw logic_error("shmget error.");

    shared_mem = shmat(shmid, NULL, 0);
    if(shared_mem == (void *)-1)
        throw logic_error("shmat error.");
}

Process::~Process()
{
    shmdt(shared_mem);
}

// 设置进程名称
void Process::SetProcessTitle(const char * title)
{
    char * p;
    const char * TITLE_PREFIX = "ucServer: ";
    os_argv[1] = NULL;

    p = strncpy(os_argv[0], TITLE_PREFIX, environ_last - os_argv[0]);
    p += strlen(p);
    p = strncpy(p, title, environ_last - p);
    p += strlen(p);

    if(environ_last - p)
        memset(p, 0, environ_last - p);
}

// 设置最大连接数
int Process::SetMaxConn(int maxconn)
{
    struct rlimit  rlmt;
    if(-1 == getrlimit(RLIMIT_NOFILE, &rlmt))
    {
        //err("getrlimit error:%m");
        return -1;
    }
    rlmt.rlim_cur = (rlim_t) maxconn;
    rlmt.rlim_max = (rlim_t) maxconn;
    if(setrlimit(RLIMIT_NOFILE, &rlmt) == -1)
    {
        //err("setrlimit error:%m");
        return -1;
    }
    return 0;
}

void* Process::getSharedMem(void)
{
    return shared_mem;
}

// 设置进程为守护进程模式
void Process::daemon(void)
{
    int fd, i;
    for(i = 0; i < 2; i++)
    {
        switch(fork())
        {
        case 0:
            break;
        case -1:
            throw logic_error("fork error.");
        default:
            exit(0);
        }
    }

    if(setsid() == -1)
        throw logic_error("setsid error.");

    umask(0);
    fd = open("/dev/null", O_RDWR);
    if(fd < 0)
        throw logic_error("open error.");

    if(dup2(fd, STDIN_FILENO) == -1)
    {
        //err("dup2 /dev/null failed:%m");
        exit(-1);
    }
    if(dup2(fd, STDOUT_FILENO) == -1)
    {
        //err("dup2 /dev/null failed:%m");
        exit(-1);
    }
    close(fd);
}

//绑定信号处理函数
void Process::SignalBind(void)
{
    for(size_t i = 0; i < sizeof(signalSets)/sizeof(signal_t); i++)
    {
        struct sigaction act = {};
        act.sa_handler = signalSets[i].handle;
        sigemptyset(&act.sa_mask);
        sigaction(signalSets[i].signo, &act, NULL);
    }
}

void Process::signal_handle(int signo)
{
    switch(signo)
    {
    case SIGSEGV:
    case SIGILL:
    case SIGFPE:
    case SIGBUS:
    {
        void * array[30];
        int size = backtrace(array, 30);
        char ** str = backtrace_symbols(array, size);

        if(!str)
        {
            Debugger::I().log(Debugger::d_pain, "backtrace_symbols failed.");
            ::exit(-1);
        }

        for(int i = 0; i < size; i++)
        {
            Debugger::I().log(Debugger::d_pain, "%s", str[i]);
        }

        free(str);
        ::exit(0);
    }
    case SIGINT:
    case SIGQUIT:
    case SIGKILL:
    case SIGTERM:
    case SIGSTOP:
        Debugger::I().log(Debugger::d_pain, "Receive STOP SIGNAL...");
        MainLoop::I().Exit();
        ::exit(-1);
    default:
        Debugger::I().log(Debugger::d_debug, "Catch Signal:%s", strsignal(signo));
    }
}
