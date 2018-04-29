#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <string>
#include <string.h>
#include <vector>
#include <set>
#include <stdint.h>

using namespace std;

namespace Util {

enum RandomStringMode
{
    ALL_LOWER,  //全部小写
    ALL_UPPER,  //全部大写
    LOWER_UPPER //大小写
};

enum ASCII
{
    ASCII_A = 65,
    ASCII_a = 97
};

//查找算法中表示查找不到
const size_t npos = -1;

//大写和小写的ascii值差值
const int UPPPER_LOWER = 'a' - 'A';

//转换为大写
void toUpper(const string& in, string& out);
void toUpper(string& inout);

//转换为小写
void toLower(const string& in, string& out);
void toLower(string& inout);

//将字符串中的所有的某个字母替换为某个字母
void repalce(const string& in, string& out, const char c_src, const char c_dst);
void repalce(string& inout, const char c_src, const char c_dst);

//从前到后查找某字符串是否包含另外一个字符串
size_t find_first_of(const char* str, const char obj, size_t pos = 0);
size_t find_first_of(const char* str, const char* obj, size_t pos = 0);

//根据分隔符将字符串划分为整型数组，返回数组个数
size_t separateString(string str, const char c, vector<int>& vecRes);
size_t separateString(string str, const char c, vector<uint32_t>& vecRes);
size_t separateString(string str, const char c, set<string>& setRes);

//根据分隔符将字符串划分为字符串数组
size_t separateString(string str, const char c, vector<string>& vecRes);

//获取随机字符串
void getRandonString(string& out, int len, int mode);

//去掉数字后面的尾缀，返回前面的整型
int getValue(string& str, const char *tail);
}

#endif  //__STRING_UTIL_H__
