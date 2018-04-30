#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include "mainloop.h"

class Server : boost::noncopyable
{
    // http句柄信息
    typedef struct _HttpContext
    {
        int fd;                     // 套接字
        struct evhttp *http_ctx;    // 根据套接字建立的http句柄
    } HttpContext;

public:
    Server();

private:
    HttpContext master_ctx_;        // Master进程套接字
    HttpContext worker_ctx_;        // Worker进程套接字
    struct event_base * evbase_;    // 异步事件控制
};

#endif // SERVER_H
