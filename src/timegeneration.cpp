#include "timegeneration.h"

TG::TG()
    : _slot_(0),
      running_(false),
      stack_(8196)
{
    m_strLocTime = (char * volatile *)new(stack_.Push<char*>()) char*(NULL);
    time_str_ = (char(*)[SLOT_MAX][STR_MAX])new(stack_.Push<char[SLOT_MAX][STR_MAX]>()) char[SLOT_MAX][STR_MAX];

    //本地时间秒数
    time_s_ = (time_t **)new(stack_.Push<time_t*>()) time_t*(NULL);
    time_s_slots_ = (time_t(*)[SLOT_MAX])new(stack_.Push<time_t[SLOT_MAX]>()) time_t[SLOT_MAX];

    //本地时间毫秒数
    time_ms_ = (unsigned long **)new(stack_.Push<unsigned long*>()) unsigned long*(NULL);
    time_ms_slots_ = (unsigned long(*)[SLOT_MAX])new(stack_.Push<unsigned long[SLOT_MAX]>()) unsigned long[SLOT_MAX];

    //格林威治时间秒数
    gmtime_s_ = (time_t **)new(stack_.Push<time_t*>()) time_t*(NULL);
    gmtime_s_slots_ = (time_t(*)[SLOT_MAX])new(stack_.Push<time_t[SLOT_MAX]>()) time_t[SLOT_MAX];

    snprintf((*time_str_)[_slot_], STR_MAX, "2015-12-20 12:00:00");
    *m_strLocTime = (char *)&(*time_str_)[_slot_];

    *time_ms_ = &(*time_ms_slots_)[_slot_];
    *time_s_ = &(*time_s_slots_)[_slot_];
    *gmtime_s_ = &(*gmtime_s_slots_)[_slot_];

    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    _gmtime[_slot_] = tv.tv_sec;
    if(tz.tz_minuteswest < 0)
    {
        tz.tz_minuteswest = -tz.tz_minuteswest;
        _time[_slot_] = tv.tv_sec + tz.tz_minuteswest * 60;
    }
    else
    {
        _time[_slot_] = tv.tv_sec - tz.tz_minuteswest * 60;
    }
    (*time_ms_slots_)[_slot_] = _time[_slot_] * 1000 + tv.tv_usec / 1000;
    (*time_s_slots_)[_slot_] = _time[_slot_];
    (*gmtime_s_slots_)[_slot_] = _gmtime[_slot_];
}

TG::~TG()
{}

void TG::Start(struct event_base * base)
{
    struct timeval tv = {0, 1000};
    assert(base != NULL);
    event_assign(&ev_time, base, -1, EV_PERSIST, TimeCallback, this);
    event_add(&ev_time, &tv);
    running_ = true;
}

void TG::TimeCallback(int fd, short event, void * args)
{
    TG * t = static_cast<TG*>(args);
    t->Update();
}

void TG::Update(void)
{
    struct timeval tv;
    struct timezone tz;
    struct tm localtm;

    gettimeofday(&tv, &tz);

    _slot_ = (_slot_ + 1) % SLOT_MAX;
    _gmtime[_slot_] = tv.tv_sec;
    if(tz.tz_minuteswest < 0)
    {
        tz.tz_minuteswest = -tz.tz_minuteswest;
        _time[_slot_] = tv.tv_sec + tz.tz_minuteswest * 60;
    }
    else
    {
        _time[_slot_] = tv.tv_sec - tz.tz_minuteswest * 60;
    }

    gmtime_r(&_time[_slot_], &localtm);

    //获得时间
    (*time_ms_slots_)[_slot_] = _time[_slot_] * 1000 + tv.tv_usec / 1000;
    (*time_s_slots_)[_slot_] = _time[_slot_];
    (*gmtime_s_slots_)[_slot_] = _gmtime[_slot_];

    //获取本地时间字符串
    snprintf((*time_str_)[_slot_], STR_MAX, "%04d-%02d-%02d %02d:%02d:%02d",
            localtm.tm_year+1900, localtm.tm_mon+1, localtm.tm_mday,
            localtm.tm_hour, localtm.tm_min, localtm.tm_sec);

    memory_barrier();
    *time_ms_ = &(*time_ms_slots_)[_slot_];
    *time_s_ = &(*time_s_slots_)[_slot_];
    *gmtime_s_ = &(*gmtime_s_slots_)[_slot_];
    *m_strLocTime = (char *)&(*time_str_)[_slot_];
}

//获取距离第二天天凌晨的时间(秒)
const long TG::getremainsecond()
{
    time_t now;
    struct tm *timenow;
    time(&now);
    timenow = localtime(&now);

    return ((23 - timenow->tm_hour) * 3600 +
            (59 - timenow->tm_min) * 60 +
            (59 - timenow->tm_sec) + 1);
}
