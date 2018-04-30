/******************************************************************************
 * filename : main.cpp
 * author   : zhaoyongfei
 * email    : zhaoyongfei@360.cn
 * desc     : 仿造ucs服务端模型搭建
******************************************************************************/

#include <getopt.h>
#include "config.h"
#include "mainloop.h"
#include "os.h"
#include "server.h"
#include "logger.h"
#include "util/os.h"

int main(int argc, char * argv[])
{
    char ch;
    //配置文件的位置
    bool daemon = false;
    const char *configPath = "./etc/base.conf";
    const char *pidPath = "./proc/ucs.pid";
    while(-1 != (ch = getopt(argc, argv, "f:dp:")))
    {
        switch(ch)
        {
        case 'f':   // 手动指定配置文件的位置
            configPath = optarg;
            break;
        case 'd':   // 是否采取守护进程方式运行程序
            daemon = true;
            break;
        case 'p':   // 指定进程文件
            pidPath = optarg;
            break;
        default:
            break;
        }
    }

    OS os(argv);
    if(daemon)
        os.daemon();
    os.SignalBind();    // 信号绑定

    // 解析配置文件
    MConfig::I().loadConfFile(configPath);

    // 初始化日志
    char debug_log_path[256];
    snprintf(debug_log_path, 255, "%s/debug", MConfig::I().log.path.data());
    Util::mkdirs(debug_log_path);
    snprintf(debug_log_path, 255, "%s/debug.master.log", debug_log_path);
    Debugger::I()->Init(MConfig::I().log.level, debug_log_path);

    // 创建服务端控制器
    Server server(&(MConfig::I().base.master_addr), &(MConfig::I().base.worker_addr));

	return 0;
}
