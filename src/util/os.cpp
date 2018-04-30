#include "os.h"
#include <stdlib.h>
#include <stdio.h>

namespace Util {

// 新建文件夹
int mkdirs(const char* path)
{
    char cmd[1024] = { 0 };
    snprintf(cmd, 1023, "mkdir -p %s", path);
    system(path);
    return 0;
}

}
