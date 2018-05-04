#pragma once

#include <cstdio>
#include <stdarg.h>
#include <stdint.h>
#include <string>
#include <rapidjson/internal/itoa.h>
using namespace rapidjson;

template<typename CHAR, int MAX=65535>
class LineBuffer
{
private:
	CHAR data_[MAX];
	mutable CHAR * buffer_;
	CHAR * bufferBeg_;
	CHAR * bufferEnd_;
	FILE * fp_;
    bool m_bCache;  //是否缓存日志
public:
    LineBuffer(FILE * fp, bool cache = true)
		: buffer_(data_), 
		bufferBeg_(data_), 
		bufferEnd_(data_+MAX), 
        fp_(fp),
        m_bCache(cache)
	{
    }

	~LineBuffer()
	{
		Flush(true);
    }

    void Flush(bool leof) const
    {
        if(leof) Put('\n');
        if(buffer_ != bufferBeg_)
        {
            size_t result = fwrite(bufferBeg_, 1, static_cast<size_t>(buffer_ - bufferBeg_), fp_);
            if(result < static_cast<size_t>(buffer_ - bufferBeg_))
            {
                //TODO?
            }
            buffer_ = bufferBeg_;

            if(! m_bCache)
            {
                fflush(fp_);
            }
        }
    }

    void Put(CHAR ch) const
    {
        if(buffer_ >= bufferEnd_)
            Flush(false);
        *buffer_++ = ch;
    }

	const LineBuffer & operator<<(const std::string & str) const
	{
		return operator<<(str.c_str());
    }

	const LineBuffer & operator<<(const CHAR * str) const
	{
		CHAR ch;
		while((ch = *str++))
		{
			if(buffer_ >= bufferEnd_)
				Flush(false);
			*buffer_++ = ch;
		}
		return *this;
    }

	const LineBuffer & operator<<(const bool b) const
	{
		if(b)
		{
			Put('t'); Put('r'); Put('u'); Put('e');
		}
		else
		{
			Put('f'); Put('a'); Put('l'); Put('s'); Put('e');
		}
		return *this;
    }

	const LineBuffer & operator<<(const int32_t i) const
	{
		char buffer[11];
		const char * end = internal::i32toa(i, buffer);
		for(const char * p = buffer; p != end; ++p)
			Put(*p);
		return *this;
    }

	const LineBuffer & operator<<(const uint32_t u) const
	{
		char buffer[10];
		const char * end = internal::u32toa(u, buffer);
		for(const char * p = buffer; p != end; ++p)
			Put(*p);
		return *this;
    }

	const LineBuffer & operator<<(const int64_t i64) const
	{
		char buffer[21];
		const char * end = internal::i64toa(i64, buffer);
		for(const char * p = buffer; p != end; ++p)
			Put(*p);
		return *this;
    }

	const LineBuffer & operator<<(const uint64_t u64) const
	{
		char buffer[20];
		const char * end = internal::u64toa(u64, buffer);
		for(const char * p = buffer; p != end; ++p)
			Put(*p);
		return *this;
    }
// nternal::dtoa找不到该函数，链接错误
//	const LineBuffer & operator<<(const double d) const
//	{
//		char buffer[25];
//		const char * end = internal::dtoa(d, buffer);
//		for(const char * p = buffer; p != end; ++p)
//			Put(*p);
//		return *this;
//	};

	const LineBuffer & Write(const char * format, ...) const
	{
		Flush(false);
		va_list ap;
		va_start(ap, format);
		int result = vsnprintf(buffer_, MAX, format, ap);
		va_end(ap);
		if(result <= MAX-1)
		{
			buffer_ += result;
		}
		else
		{
			buffer_ += MAX-1;
		}
		return *this;
    }

	const LineBuffer & VWrite(const char * format, va_list ap) const
	{
		Flush(false);
		int result = vsnprintf(buffer_, MAX, format, ap);
		if(result <= MAX-1)
		{
			buffer_ += result;
		}
		else
		{
			buffer_ += MAX-1;
		}
		return *this;
    }
};
