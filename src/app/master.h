#ifndef MASTER_H
#define MASTER_H

#include "application.h"

class MasterApp : public BaseApp
{
public:
    MasterApp();

    virtual ~MasterApp();

    virtual void Init();

private:
    // 函数功能: 健康检查窗口
    static void HealthCheckCallback(struct evhttp_request * req, void * arg);
};

#endif // MASTER_H
