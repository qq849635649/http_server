#include "os_util.h"
#include <stdlib.h>
#include <stdio.h>
#include <md5/md5.h>

namespace Util {

// 新建文件夹
int mkdirs(const char* path)
{
    char cmd[1024] = { 0 };
    snprintf(cmd, 1023, "mkdir -p %s", path);
    system(cmd);
    return 0;
}

int md5sum(const char* filename, string &res)
{
    unsigned char digest[16] = {0};
    char out[64];
    unsigned char buf[4096];
    unsigned int len = 0, ret = 0;

    MD5_CTX context;
    MD5Init(&context);

    /* 计算文件MD5 */
    FILE *fp = fopen(filename, "r");
    if(NULL == fp)
        return -1;

    while ((len = fread (buf, 1, 4096, fp)) != 0)
    {
        MD5Update(&context, buf, len);
    }
    MD5Final(&context, digest);
    fclose(fp);
    for(int i = 0; i < 16; ++i)
        ret += sprintf(out + i * 2, "%02x", digest[i]);
    res.insert(0, out, ret);

    return 0;
}

}
