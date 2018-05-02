#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include "linebuffer.h"
#include "filehandler.h"
#include "singleton.h"

#define err(fmt, ...)	fprintf(stderr, "[%s +%d]"fmt"\n", __func__, __LINE__, ##__VA_ARGS__)
#define debug(level, fmt, ...) _debug(level, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define PANIC(fmt, ...)	do{fprintf(stderr, fmt"\n", ##__VA_ARGS__); exit(-1);}while(0)
#define FILENAME_LEN (128)

///////////////////////////////////////////////////////////////////////////////
/// class Debugger
/// 类说明: 调试日志，采用的模式为单例模式

class Debugger : public Singleton<Debugger>
{
    friend class Singleton<Debugger>;
public:
    enum
    {
        d_pain,
        d_debug,
        d_info,
        D_MAX_LEVEL
    };

    //初始化日志，也可以用来切换日志文件名称
    void Init(int level, const char * path);

    //写入日志日志
    void log(int level, const char * fmt, ...);

private:
    Debugger();
    ~Debugger();

    int debug_level;
    FILE * handle;
    static Debugger* _debug;
    Debugger(const Debugger & d);
    Debugger & operator=(const Debugger & d);
};

///////////////////////////////////////////////////////////////////////////////
/// class TimeRotateLogger
/// 类说明: 带有自动切换文件功能的日志类

class TimeRotateLogger
{
private:
    std::string name_;
    FileHandler file_;
    bool m_bCache;  //是否缓存日志，默认缓存
    char file_name_timestamp_[FILENAME_LEN];
    typedef LineBuffer<char> CharLineBuffer;

    //切换文件时候获取老文件的文件名
    const std::string CalcFileName() const;
    void RefreshFileName();
public:
    TimeRotateLogger(const std::string & name, bool cache = true);
    ~TimeRotateLogger();

    //记录日志
    CharLineBuffer Info(const char * format, ...);
    CharLineBuffer Info();

    //切换日志文件
    void ReOpenFile();
};

#endif  //__MODULE_DEBUG_H__
