#ifndef UTIL_H
#define UTIL_H

#include <string>
using namespace std;

namespace Util {

// 新建文件夹
int mkdirs(const char *path);

//计算文件的md5值
int md5sum(const char* filename, string &res);

}

#endif // UTIL_H
