#pragma once
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <boost/noncopyable.hpp>
#include <string>
#include "singleton.h"

namespace Hund
{
namespace details
{

typedef unsigned char	u_char;
typedef uintptr_t	ncx_uint_t;
typedef intptr_t	ncx_int_t;

typedef struct ncx_slab_page_s  ncx_slab_page_t;
struct ncx_slab_page_s
{
    uintptr_t         slab;
    ncx_slab_page_t  *next;
    uintptr_t         prev;
};

typedef struct
{
	size_t            min_size;
	size_t            min_shift;

	ncx_slab_page_t  *pages;
	ncx_slab_page_t   free;

	u_char           *start;
	u_char           *end;
	void             *addr;
}ncx_slab_pool_t;

typedef struct
{
	size_t 			pool_size, used_size, used_pct; 
	size_t			pages, free_page;
	size_t			p_small, p_exact, p_big, p_page; /* 四种slab占用的page数 */
	size_t			b_small, b_exact, b_big, b_page; /* 四种slab占用的byte数 */
	size_t			max_free_pages;					 /* 最大的连续可用page数 */
}ncx_slab_stat_t;

void ncx_slab_init(ncx_slab_pool_t *pool);
void * ncx_slab_alloc_locked(ncx_slab_pool_t *pool, size_t size);
void ncx_slab_free_locked(ncx_slab_pool_t *pool, void *p);
void ncx_slab_stat(ncx_slab_pool_t *pool, ncx_slab_stat_t *stat);
}

class SlabException
{
private:
	std::string error_;
public:
	SlabException(std::string string):error_(string)
	{
    }

	const char * what()
	{
		return error_.c_str();
    }
};

class Slabs : public Singleton<Slabs>
{
private:
	details::ncx_slab_pool_t * pool;

protected:
	friend class Singleton<Slabs>;
    Slabs() { }
    ~Slabs() { }
public:
	void Init(void * memory, const size_t size)
	{
		pool = reinterpret_cast<details::ncx_slab_pool_t *>(memory);
		pool->addr = memory;
		pool->min_shift = 3;
		pool->end = reinterpret_cast<details::u_char *>(memory) + size;
		details::ncx_slab_init(pool);
    }

	template <typename T>
	T * Malloc(size_t size = 1)
	{
		T * ptr = static_cast<T *>(details::ncx_slab_alloc_locked(pool, size*sizeof(T)));
		if(ptr == NULL)
			throw SlabException("No More Memory");
		return ptr;
    }

	template <typename T>
	T * Calloc(size_t size = 1)
	{
		T * ptr = Malloc<T>(size);
		memset(ptr, 0, size * sizeof(T));
		return ptr;
    }

	void Free(void * ptr)
	{
		details::ncx_slab_free_locked(pool, ptr);
    }
};
}
