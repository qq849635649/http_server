#ifndef __M_TIME_H__
#define __M_TIME_H__

#include <event.h>
#include "singleton.h"
#include "./shm/sharedstack.h"

#define SLOT_MAX	32
#define STR_MAX		50

class TG : public Singleton<TG>
{
    friend class Singleton<TG>;
public:
    void Start(struct event_base * base);

    const char * GetTime() const
    {
        return *m_strLocTime;
    }

    const unsigned long GetTimeMs() const
    {
        return **time_ms_;
    }

    const time_t GetTimeS() const
    {
        return **time_s_;
    }

protected:
    TG();

    ~TG();

    void Update(void);

    static void TimeCallback(int fd, short event, void * args);

private:
    unsigned int _slot_;
    unsigned long * volatile * time_ms_;
    time_t * volatile * time_s_;
    time_t _time[SLOT_MAX];
    time_t _gmtime[SLOT_MAX];
    unsigned long (*time_ms_slots_)[SLOT_MAX];
    time_t (*time_s_slots_)[SLOT_MAX];
    char (* time_str_)[SLOT_MAX][STR_MAX];//本地时间字缓冲区
    char * volatile * m_strLocTime;	//本地时间的字符串
    struct event ev_time;
    SharedStackAllocator stack_;
};

#endif
