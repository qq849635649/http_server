#include "server.h"
#include "logger.h"
#include <netinet/tcp.h>
#include <fcntl.h>
#include <stdexcept>

using namespace std;

#define SOCKET_BUF_MAX	(64 * 1024 * 1024)

int Server::sequence_ = 0;

Server::Server(struct sockaddr_in *master_addr, struct sockaddr_in *worker_addr)
{
    assert(worker_addr && master_addr);
    AddListeners(master_addr, &master_ctx_);
    AddListeners(worker_addr, &worker_ctx_);
}

// 创建套接字
void Server::AddListeners(sockaddr_in *addr, HttpContext *http_ctx)
{
    int on = 1;
    int idle = 480;
    int intvl = 150;
    int cnt = 4;

    int handle = socket(AF_INET, SOCK_STREAM, 0);
    if(handle < 0)
        throw logic_error("create handle error.");

    if(evutil_make_socket_nonblocking(handle) < 0)
        throw logic_error("evutil_make_socket_nonblocking error.");

    SetMaxSocketBuf(handle, SOCKET_BUF_MAX, SO_RCVBUF);
    SetMaxSocketBuf(handle, SOCKET_BUF_MAX, SO_SNDBUF);

    if(setsockopt(handle, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on)) < 0)
        throw logic_error("set SO_KEEPALIVE error.");

    if(setsockopt(handle, SOL_TCP, TCP_KEEPIDLE, (void *)&idle, sizeof(idle)) < 0)
        Debugger::I().log(Debugger::d_debug, "set TCP_KEEPIDLE error:%m");

    if(setsockopt(handle, SOL_TCP, TCP_KEEPINTVL, (void *)&intvl, sizeof(intvl)) < 0)
        Debugger::I().log(Debugger::d_debug, "set TCP_KEEPINTVL error:%m");

    if(setsockopt(handle, SOL_TCP, TCP_KEEPCNT, (void *)&cnt, sizeof(cnt)) < 0)
        Debugger::I().log(Debugger::d_debug, "set TCP_KEEPCNT error:%m");

    if(evutil_make_listen_socket_reuseable(handle) < 0)
        throw logic_error("set reuse error.");

    if(bind(handle, (struct sockaddr *)addr, sizeof(*addr)) < 0)
        throw logic_error("bind error.");

    if(listen(handle, 511) < 0)
        throw logic_error("listen error.");
    http_ctx->fd = handle;
}

//设置tcp参数
int Server::SetMaxSocketBuf(int fd, int buff_max, int opt)
{
    socklen_t intsize = sizeof(int);
    int min, max, avg;
    int old_size;

    if(getsockopt(fd, SOL_SOCKET, opt, &old_size, &intsize) != 0)
    {
        Debugger::I().log(Debugger::d_debug, "getsockopt error:%m");
        return -1;
    }

    min = old_size;
    max = buff_max;

    while (min <= max)
    {
        avg = ((unsigned int)(min + max)) / 2;
        if(setsockopt(fd, SOL_SOCKET, opt, (void *)&avg, intsize) == 0)
            min = avg + 1;
        else
            max = avg - 1;
    }
    return 0;
}

// 启动子进程
int Server::GenerateWorker(int workers)
{
    for(int i = 0; i < workers; i++)
    {
        int pipe[2] = {};
        socketpair(AF_UNIX, SOCK_STREAM, 0, pipe);
        pid_t pid = fork();
        switch(pid)
        {
        case -1:
            throw logic_error("Generate Worker error");
            break;
        case 0:
            type_ = P_WORKER;
            sequence_ = i;
            close(pipe[0]);
            controle_ = pipe[1];
            return type_;
        default:
            pids_.push_back(pid);
            type_ = P_MASTER;
            close(pipe[1]);
            sub_pipes_.push_back(pipe[0]);
        }
    }
    return type_;
}

// 将pid保存到pid文件中
void Server::PidFile(const char* pidfile)
{
    int fd, ret;
    pid_t pid;
    char buff[200] = {};

    fd = open(pidfile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if(fd < 0)
        Debugger::I().log(Debugger::d_debug, "open %s :%m", pidfile);

    pid = getpid();
    ret = snprintf(buff, 200, "%d\n", pid);
    ret = write(fd, buff, ret);
    if(ret < 0)
        Debugger::I().log(Debugger::d_debug, "write %s err:%m", pidfile);

    for(vector<pid_t>::size_type i = 0; i < pids_.size(); i++)
    {
        ret = snprintf(buff, 200, "%d\n", pids_[i]);
        ret = write(fd, buff, ret);
        if(ret < 0)
            Debugger::I().log(Debugger::d_debug, "write %s err:%m", pidfile);
    }
    close(fd);
}
