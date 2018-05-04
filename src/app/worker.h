#ifndef WORKER_H
#define WORKER_H

#include "application.h"

class WorkerApp : public BaseApp
{
public:
    WorkerApp();
    virtual ~WorkerApp();

    virtual void Init();

private:
    // 函数功能: 健康检查窗口
    static void HealthCheckCallback(struct evhttp_request * req, void * arg);
};

#endif // WORKER_H
