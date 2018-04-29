#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
using namespace  std;

#include <sys/types.h>
#include <netinet/in.h>
#include <libconfig/libconfig.h>
#include "singleton.h"

class MConfig : public Singleton<MConfig>
{
    friend class Singleton<MConfig>;
public:
    struct Base
    {
        struct sockaddr_in worker_addr; // worker监听的端口
        struct sockaddr_in master_addr; // master监听的端口
        int workers;                    // worker进程的个数
        int maxconn;                    // 最大允许的并行数
        int shared_memory;              // 共享内存的大小
    } base;

    struct Log
    {
        string path;                    // 日志所在的目录
        int32_t level;                  // debug日志级别(pain/debug/info)
    } log;

    void loadConfFile(const char *file);

private:
    // 加载IP信息
    int getSockAddr(config_t* setting, const char *conf, struct sockaddr_in *addr);
};

#endif // __CONFIG_H__
