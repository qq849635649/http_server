#include <stdexcept>
using namespace std;

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "config.h"

MConfig::MConfig()
{
}

// 加载配置文件
void MConfig::loadConfFile(const char *file)
{
    config_t setting = {};
    config_init(&setting);

    //const char* tmp = NULL;

    if(CONFIG_TRUE != config_read_file(&setting, file))
        throw invalid_argument("Read configure error.");

    // 获取worker监听端口
    if(getSockAddr(&setting, "Base.worker", &(base.worker_addr)))
        throw invalid_argument("Find Base.worker error.");

    // 获取master监听端口
    if(getSockAddr(&setting, "Base.master", &(base.worker_addr)))
        throw invalid_argument("Find Base.master error.");

    // 获取worker子进程数量
    if(CONFIG_TRUE != config_lookup_int(&setting, "Base.worker_processes", &(base.workers)))
        throw invalid_argument("Find Base.worker_processes error.");

    // 获取开辟的共享内存空间大小
    if(CONFIG_TRUE != config_lookup_int(&setting, "Base.shared_memory", &(base.shared_memory)))
        throw invalid_argument("Find Base.shared_memory error.");

    // 获取当前最大连接数
    if(CONFIG_TRUE != config_lookup_int(&setting, "Base.maxconn", &(base.maxconn)))
        base.maxconn = 65535;

    config_destroy(&setting);
}

// 加载IP信息
int MConfig::getSockAddr(config_t *setting, const char *conf, struct sockaddr_in *addr)
{
    const char* listen = NULL;
    char ip[30] = {};
    int port = 0;
    if(CONFIG_TRUE != config_lookup_string(setting, conf, &listen))
        return -1;

    char * delim = strchr(const_cast<char *>(listen), ':');
    if(delim)
    {
        strncpy(ip, listen, delim - listen);
        port = atoi(++delim);
    }
    else
        return -1;

    if(inet_pton(AF_INET, ip, &(addr->sin_addr)) <= 0)
        return -1;

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    return 0;
}
