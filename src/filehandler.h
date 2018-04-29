#pragma once

#include <cstdio>
#include <string>
#include <stdexcept>
#include <boost/noncopyable.hpp>

class FileHandler : boost::noncopyable
{
private:
	FILE * fp_;
	std::string name_;
public:
	FileHandler() : fp_(NULL)
	{
    }

	~FileHandler()
	{
		Close();
    }

	void Open(std::string name)
	{
		Close();
		fp_ = fopen(name.c_str(), "ab");
		if(!fp_)
        {
            throw std::runtime_error("open " + name + " failed.");
        }

		name_ = name;
    }

	void Close()
	{
		if(fp_)
		{
			fclose(fp_);
			name_.clear();
			fp_ = NULL;
		}
    }

	FILE * GetPointer() const
	{
		return fp_;
	}

	const std::string & FileName() const
	{
		return name_;
    }
};
