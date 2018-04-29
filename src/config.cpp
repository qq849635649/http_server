#include <stdexcept>
using namespace std;

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "config.h"

MConfig::MConfig()
{
}

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

    config_destroy(&setting);
}

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
