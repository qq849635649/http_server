#pragma once
#include <sys/mman.h>
#include <assert.h>
#include <stdint.h>
#include <boost/noncopyable.hpp>
#include "logger.h"

class SharedStackAllocator : boost::noncopyable
{
private:
	char * memory_;
	char * stack_top_;
	char * stack_end_;
	size_t free_;

	void * alloc(void * v, unsigned long int size)
	{
		if (sizeof(uintptr_t) == 8 && size > (1L<<31))
		{
			void * p = mmap(v, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
			if (p != v)
			{
				if (p != MAP_FAILED)
					munmap(p, size);
				return MAP_FAILED;
			}
			return p;
		}

		return mmap(v, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
    }
public:
	SharedStackAllocator(unsigned long int SIZE)
	{
		for (uintptr_t i = 0 ; i <= 0x7f; i++)
		{
#ifdef __x86_64__
			uintptr_t p = (i<<40) | (0xFFFFFFFFFFFFFFFF&((uintptr_t)0x0040<<32));
#else
			uintptr_t p = 0;
#endif
			memory_ = (char *)alloc((void *)p, SIZE);
			if (memory_ != MAP_FAILED)
			{
				stack_top_ = memory_;
				stack_end_ = memory_ + SIZE;
				free_ = SIZE;
				return;
			}
		}

		PANIC("Cannot alloc shared memory.");
    }

	~SharedStackAllocator()
	{
		munmap(memory_, stack_end_ - memory_);
    }

	template<typename T>
	T * Push(size_t count = 1)
	{
		assert(stack_top_ + sizeof(T) * count <= stack_end_);
		T * ret = reinterpret_cast<T*>(stack_top_);
		stack_top_ += sizeof(T)*count;
		free_ -= sizeof(T)*count;
		return ret;
    }

	size_t GetFree()
	{
		return free_;
    }
};
