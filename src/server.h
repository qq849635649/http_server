#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include "mainloop.h"

#define MASTER_CONTEXT  0           // master进程的业务
#define WORKER_CONTEXT  1           // worker进程的业务

// 服务端控制中心
class Server : boost::noncopyable
{
    // http句柄信息
    typedef struct _HttpContext
    {
        int fd;                     // 套接字
        struct evhttp *ctx;         // 根据套接字建立的http句柄
    } HttpContext;

public:
    Server(struct sockaddr_in *master_addr,
           struct sockaddr_in *worker_addr);

private:
    // 创建套接字
    void AddListeners(struct sockaddr_in *addr, HttpContext *http_ctx);
    //设置tcp参数
    int SetMaxSocketBuf(int fd, int buff_max, int opt);

    HttpContext master_ctx_;        // Master进程套接字
    HttpContext worker_ctx_;        // Worker进程套接字
    struct event_base *evbase_;     // 异步事件控制
};

#endif // SERVER_H
