
#include <getopt.h>
#include "config.h"

int main(int argc, char * argv[])
{
    char ch;
    //配置文件的位置
    bool daemon = false;
    const char *configPath = "./etc/base.conf";
    const char *pidPath = "./proc/ucs.pid";
    while(-1 != (ch = getopt(argc, argv,
            "f:"    //手动指定配置文件的位置
            "d"     //是否采取守护进程方式运行程序
            "p:"	//进程文件
            )))
    {
        switch(ch)
        {
        case 'f':
            configPath = optarg;
            break;
        case 'd':
            daemon = true;
            break;
        case 'p':
            pidPath = optarg;
            break;
        default:
            break;
        }
    }

    if(daemon) {}

    // 解析配置文件
    MConfig::I().loadConfFile(configPath);

	return 0;
}
