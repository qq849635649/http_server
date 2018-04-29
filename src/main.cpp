
#include <getopt.h>
#include "config.h"
#include "mainloop.h"
#include "os.h"

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

    // 解析配置文件
    MConfig::I().loadConfFile(configPath);

    // 初始化日志

	return 0;
}
