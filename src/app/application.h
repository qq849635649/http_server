#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "../logger.h"
#include "../mainloop.h"

#include <map>
using namespace std;

class BaseApp
{
public:
    BaseApp() {}
    virtual ~BaseApp() {}

    virtual void Init() = 0;

    typedef void (*ReqHandler)(struct evhttp_request *, void *);
    typedef map<string, ReqHandler>::const_iterator const_HandlerIter;

    // 获取uri与回调函数的映射
    const map<string, ReqHandler>& GetHandlers() const
    {
        return handlers_;
    }

protected:
    map<string, ReqHandler> handlers_;
};

#endif  // __APPLICATION_H__
