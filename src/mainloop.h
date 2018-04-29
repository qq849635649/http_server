#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <assert.h>
#include <event2/event-config.h>
#include <event2/event.h>
#include "singleton.h"

class MainLoop : public Singleton<MainLoop>
{
    struct event_base * base_;
    friend class Singleton<MainLoop>;

private:
    MainLoop()
        : base_(NULL){}

    ~MainLoop()
    {
        if(base_)
            event_base_free(base_);
    }

public:
    void Init()
    {
        base_ = event_base_new();
        assert(base_ != NULL);
    }

    void Run()
    {
        assert(base_ != NULL);
        event_base_dispatch(base_);
    }

    void Exit()
    {
        assert(base_ != NULL);
        event_base_loopexit(base_, NULL);
    }

    struct event_base * GetBase()
    {
        assert(base_ != NULL);
        return base_;
    }
};

#endif // MAINLOOP_H
