#ifndef UTIL_H
#define UTIL_H

namespace Util {

// 新建文件夹
int mkdirs(const char *path);

//新建 dir/path 文件夹
int mksubdirs(const char *dir, const char *sub);
}

#endif // UTIL_H
