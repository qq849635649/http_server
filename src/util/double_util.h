#ifndef DOUBLE_UTIL_H
#define DOUBLE_UTIL_H

using namespace std;
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>

namespace Util {

//根据cpm价格，获取单词广告价格(无精度差)
void cpm_price(int32_t cpm, char* price, int len);

}

#endif // DOUBLE_UTIL_H
