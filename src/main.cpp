/******************************************************************************
 * filename : main.cpp
 * author   : zhaoyongfei
 * email    : zhaoyongfei@360.cn
 * desc     : 仿造ucs服务端模型搭建
******************************************************************************/

#include <getopt.h>
#include "config.h"
#include "mainloop.h"
#include "process.h"
#include "server.h"
#include "logger.h"
#include "util/os_util.h"

// app
#include "app/application.h"
#include "app/master.h"
#include "app/worker.h"

int main(int argc, char * argv[])
{
    char ch;
    //配置文件的位置
    bool daemon = false;
    const char *configPath = "./etc/base.conf"; // 默认配置文件
    const char *pidPath = "./proc/ucs.pid";     // 默认的pid文件
    while(-1 != (ch = getopt(argc, argv, "f:dp:th")))
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
        case 't':   // 检测配置文件是否可用
            return 0;
        case 'h':   // 打印帮助信息
            break;
        default:
            break;
        }
    }

    Process proc(argv);
    if(daemon)
        proc.daemon();
    proc.SignalBind();    // 信号绑定

    // 解析配置文件
    MConfig::I().loadConfFile(configPath);

    // 初始化日志
    char debug_log_path[256];
    snprintf(debug_log_path, 255, "%s/debug", MConfig::I().log.path.data());
    Util::mkdirs(debug_log_path);
    snprintf(debug_log_path, 255, "%s/debug.master.log", debug_log_path);
    Debugger::I().Init(MConfig::I().log.level, debug_log_path);

    // 创建服务端控制器
    Server server(&(MConfig::I().base.master_addr), &(MConfig::I().base.worker_addr));
    switch(server.GenerateWorker(MConfig::I().base.workers))
    {
    case P_MASTER:
        server.PidFile(pidPath);        //将pid存入文件
        proc.SetProcessTitle("master");
        break;
    case P_SINGLE:
        server.PidFile(pidPath);        //将pid存入文件
        proc.SetProcessTitle("single");
        break;
    case P_WORKER:
        proc.SetProcessTitle("worker");
        break;
    }

    Debugger::I().log(Debugger::d_pain, "START WORKING...");
    MainLoop::I().Run();
    Debugger::I().log(Debugger::d_pain, "STOP WORKING...");

	return 0;
}
