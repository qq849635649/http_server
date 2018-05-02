#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include "mainloop.h"

#include <vector>
using namespace std;

#define MASTER_CONTEXT  0           // master进程的业务
#define WORKER_CONTEXT  1           // worker进程的业务

#define P_MASTER	1
#define P_SINGLE	2
#define P_WORKER	3

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
    Server(struct sockaddr_in *master_addr, struct sockaddr_in *worker_addr);

    //启动子进程
    int GenerateWorker(int workers);

private:
    // 创建套接字
    void AddListeners(struct sockaddr_in *addr, HttpContext *http_ctx);
    //设置tcp参数
    int SetMaxSocketBuf(int fd, int buff_max, int opt);

    HttpContext master_ctx_;        // Master进程套接字
    HttpContext worker_ctx_;        // Worker进程套接字
    struct event_base *evbase_;     // 异步事件控制
    vector<int> pids_;              // 子进程的进程号列表
    vector<int> sub_pipes_;         // master进程持有子进程管道列表
    int controle_;                  // worker进程持有的管道，用于与master通信
    int type_;                      // 进程类型
    static int sequence_;           // worker进程序号
};

#endif // SERVER_H
