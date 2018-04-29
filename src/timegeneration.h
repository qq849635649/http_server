#ifndef __M_TIME_H__
#define __M_TIME_H__
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <event.h>
#include <new>
#include <string>
#include <string.h>
#include <boost/noncopyable.hpp>
#include "singleton.h"
#include "sharedstack.h"

using namespace std;

#define SLOT_MAX	32
#define STR_MAX		50
#define memory_barrier()    __asm__ volatile ("" ::: "memory")

class TG : public Singleton<TG>
{
public:
    void Start(struct event_base * base);

    //获得格林威治时间秒数
    const time_t GetGmTimeS() const
    {
        return **gmtime_s_;
    }

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

    //获取距离第二天天凌晨的时间(秒)
    static const long getremainsecond();

protected:
    friend class Singleton<TG>;
    TG();

    ~TG();

    void Update(void);

    static void TimeCallback(int fd, short event, void * args);

private:
    static const char * months_[];

    unsigned int _slot_;
    bool running_;
    unsigned long * volatile * time_ms_;
    time_t * volatile * time_s_;
    time_t * volatile * gmtime_s_;  //格林威治时间
    time_t _time[SLOT_MAX];
    time_t _gmtime[SLOT_MAX];
    unsigned long (*time_ms_slots_)[SLOT_MAX];
    time_t (*time_s_slots_)[SLOT_MAX];
    time_t (*gmtime_s_slots_)[SLOT_MAX];    //格林威治时间缓冲区
    char (* time_str_)[SLOT_MAX][STR_MAX];//本地时间字缓冲区
    char * volatile * m_strLocTime;	//本地时间的字符串
    struct event ev_time;
    SharedStackAllocator stack_;
};

#endif
