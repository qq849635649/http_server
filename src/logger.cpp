#include "logger.h"
#include "timegeneration.h"
#include <stdexcept>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// class Debugger

Debugger* Debugger::_debug = NULL;

Debugger::Debugger()
    : debug_level(-1),
      handle(NULL)
{
}

Debugger::~Debugger()
{
    fclose(handle);
}

//获取调试日志句柄
Debugger* Debugger::I()
{
    if(_debug == NULL)
        _debug = new Debugger;

    return _debug;
}

//初始化日志，也可以用来切换日志文件名称
void Debugger::Init(int level, const char * path)
{
    debug_level = level + 1;

    if(handle != NULL)
        fclose(handle);

    handle = fopen(path, "a+");
    if(! handle)
    {
        err("fopen %s %m", path);
        exit(-1);
    }
}

//写入日志日志
void Debugger::log(int level, const char * fmt, ...)
{
    if(-1 == debug_level)
    {
        throw logic_error("Debug log has not be initialed...");
    }

    const char* strLevel = NULL;

    switch(level)
    {
    case d_pain:
        strLevel = "d_pain";
        break;
    case d_debug:
        strLevel = "d_debug";
        break;
    case d_info:
        strLevel = "d_info";
        break;
    default:
        strLevel = "default";
        break;
    }

    if(level < debug_level)
    {
        va_list ap;
        va_start(ap, fmt);
        fprintf(handle, "[%8d] [%s] [%s]", getpid(), TG::I().GetTime(), strLevel);
        vfprintf(handle, fmt, ap);
        fprintf(handle, "\n");
        va_end(ap);
        fflush(handle);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// class TimeRotateLogger

TimeRotateLogger::TimeRotateLogger(const std::string & name, bool cache)
    : name_(name),
      m_bCache(cache)
{
    file_.Open(name_);
    RefreshFileName();  //启动之初先计算出文件名称
}

TimeRotateLogger::~TimeRotateLogger() { }

//切换文件时候获取老文件的文件名
const std::string TimeRotateLogger::CalcFileName() const
{
    char buffer[1024];
    struct tm tm_time;
    time_t time_now = time(NULL);
        localtime_r(&time_now, &tm_time);
    snprintf(buffer, 1024, "%s_%04d-%02d-%02d_%02d_%02d",
            name_.c_str(),
            tm_time.tm_year + 1900,
            tm_time.tm_mon + 1,
            tm_time.tm_mday,
            tm_time.tm_hour,
            (tm_time.tm_min/10)*10);
    return std::string(buffer);
}

void TimeRotateLogger::RefreshFileName()
{
    memset(file_name_timestamp_, 0, FILENAME_LEN);
    struct tm tm_time;
    time_t time_now = time(NULL);
    localtime_r(&time_now, &tm_time);
    snprintf(file_name_timestamp_, FILENAME_LEN-1, "%s_%04d-%02d-%02d_%02d_%02d",
             name_.c_str(), tm_time.tm_year + 1900, tm_time.tm_mon + 1,
             tm_time.tm_mday, tm_time.tm_hour, (tm_time.tm_min/10)*10);
}

//记录日志
TimeRotateLogger::CharLineBuffer TimeRotateLogger::Info(const char * format, ...)
{
    CharLineBuffer buffer(file_.GetPointer(), m_bCache);
    va_list ap;
    va_start(ap, format);
    buffer.VWrite(format, ap);
    va_end(ap);
    return buffer;
}

//记录空日志，仅包含时间
TimeRotateLogger::CharLineBuffer TimeRotateLogger::Info()
{
    CharLineBuffer buffer(file_.GetPointer(), m_bCache);
    buffer.Write("%s ", TG::I().GetTime());
    return buffer;
}

//切换日志文件
void TimeRotateLogger::ReOpenFile()
{
    file_.Close();
    rename(name_.c_str(), file_name_timestamp_);
    file_.Open(name_);
    RefreshFileName();
}

