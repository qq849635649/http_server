#include "string_util.h"
#include <stdlib.h>
#include <iostream>

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace Util {

//转换为大写
void toUpper(const string& in, string& out)
{
    for(size_t i = 0; i < in.size(); i++)
    {
        if(in[i] >= 'a' && in[i] <= 'z')
        {
            out += (in[i] - UPPPER_LOWER);
        }
        else
            out += in[i];
    }
}

void toUpper(string& inout)
{
    string out;
    toUpper(inout, out);
    inout = out;
}

//转换为小写
void toLower(const string& in, string& out)
{
    for(size_t i = 0; i < in.size(); i++)
    {
        if(in[i] >= 'A' && in[i] <= 'Z')
        {
            out += (in[i] + UPPPER_LOWER);
        }
        else
            out += in[i];
    }
}

void toLower(string& inout)
{
    string out;
    toLower(inout, out);
    inout = out;
}

//将字符串中的所有的某个字母替换为某个字母
void repalce(const string& in, string& out, const char c_src, const char c_dst)
{
    for(size_t i = 0; i < in.size(); i++)
    {
        if(in[i] == c_src)
            out += c_dst;
        else
            out += in[i];
    }
}

void repalce(string& inout, const char c_src, const char c_dst)
{
    string out;
    repalce(inout, out, c_src, c_dst);
    inout = out;
}

//从前到后查找某字符是否包含另外一个字符串
size_t find_first_of(const char* str, const char obj, size_t pos)
{
    if(NULL == str)
        return -1;

    for(size_t i = pos; i < strlen(str); i++)
    {
        if(*(str + i) == obj)
            return i;
    }

    return -1;
}

//从前到后查找某字符串是否包含另外一个字符串
size_t find_first_of(const char* str, const char* obj, size_t pos)
{
    if(NULL == str || NULL == obj)
        return -1;

    for(size_t i = pos; i < strlen(str) && i < strlen(obj); i++)
    {
        if(strncmp(str + i, obj, strlen(obj)) == 0)
            return i;
    }

    return -1;
}

//根据分隔符将字符串划分为整型数组，返回数组个数
size_t separateString(string str, const char c, vector<int>& vecRes)
{
    while( true )
    {
        size_t pos = find_first_of(str.data(), c, 0);
        if(pos != npos)
        {
            string substr = str.substr(0, pos);
            if(! substr.empty())
            {
                vecRes.push_back(atoi(substr.data()));
            }
            str = str.substr(pos + 1);
        }
        else if(! str.empty())
        {
            vecRes.push_back(atoi(str.data()));
            break;
        }
        else
            break;
    }

    return vecRes.size();
}

size_t separateString(string str, const char c, vector<uint32_t>& vecRes)
{
    while( true )
    {
        size_t pos = find_first_of(str.data(), c, 0);
        if(pos == npos)
        {
            if(! str.empty())
            {
                try
                {
                    uint32_t value = lexical_cast<uint32_t>(str.data());
                    vecRes.push_back(value);
                }
                catch(bad_lexical_cast&)
                {
                    break;
                }
            }

            break;
        }
        else
        {
            string substr = str.substr(0, pos);
            if(! substr.empty())
            {
                try
                {
                    uint32_t value = lexical_cast<uint32_t>(substr.data());
                    vecRes.push_back(value);
                }
                catch(bad_lexical_cast&)
                {
                    str = str.substr(pos + 1);
                    continue;
                }
            }
            str = str.substr(pos + 1);
        }
    }

    return vecRes.size();
}

//根据分隔符将字符串划分为字符串数组
size_t separateString(string str, const char c, vector<string>& vecRes)
{
    while( true )
    {
        size_t pos = find_first_of(str.data(), c, 0);
        if(pos != npos)
        {
            string substr = str.substr(0, pos);
            if(! substr.empty())
            {
                vecRes.push_back(substr);
            }
            str = str.substr(pos + 1);
        }
        else if(! str.empty())
        {
            vecRes.push_back(str);
            break;
        }
        else
            break;
    }

    return vecRes.size();
}

size_t separateString(string str, const char c, set<string>& setRes)
{
    while( true )
    {
        size_t pos = find_first_of(str.data(), c, 0);
        if(pos != npos)
        {
            string substr = str.substr(0, pos);
            if(! substr.empty())
            {
                setRes.insert(substr);
            }
            str = str.substr(pos + 1);
        }
        else if(! str.empty())
        {
            setRes.insert(str);
            break;
        }
        else
            break;
    }

    return setRes.size();
}

void getRandonString(string& out, int len, int mode)
{
    char* buf = new char[len + 1];
    memset(buf, 0, len + 1);
    if(NULL != buf)
    {
        for(int i = 0; i < len; i++)
        {
            char c = rand() % 26;
            char n = -1;
            switch(mode)
            {
            case ALL_LOWER:
                c += ASCII_a;
                break;
            case ALL_UPPER:
                c += ASCII_A;
                break;
            case LOWER_UPPER:
            default:
                n = rand() % 2;
                c += ((n % 2 == 0) ? ASCII_A : ASCII_a);
                break;
            }
            *(buf + i) = c;
        }
        out = buf;
        delete[] buf;
    }
}

//去掉数字后面的尾缀，返回前面的整型
int getValue(string& str, const char* tail)
{
    if(NULL == tail)
        return 0;

    string obj = string(tail);
    std::string::size_type pos = str.find(tail);
    if (pos != str.npos)
    {
        str = str.substr(0, pos);
        return atoi(str.data());
    }
    else
        return atoi(str.data());
}

}
/*
#include <iostream>
using namespace std;

int main()
{
    string str = "720px";
    cout << Util::getValue(str, "px") << endl;
}*/
