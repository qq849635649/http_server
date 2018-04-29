#include "double_util.h"
#include <stdio.h>

namespace Util {

//根据cpm价格，获取单词广告价格(无精度差)
void cpm_price(int32_t cpm, char* price, int len)
{
    int strlen = snprintf(price , len-1,
                          "%1d.%1d%1d%1d",
                          cpm / 1000,
                          cpm % 1000 / 100,
                          cpm % 100 / 10,
                          cpm % 10);
    price[strlen] = '\0';
}

}
