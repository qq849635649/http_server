#pragma once
#include <sched.h>
#include "sharedstack.h"

#define atomic_cmp_set(lock, old, set)	__sync_bool_compare_and_swap(lock, old, set)
#define atomic_fetch_add(value, add) __sync_fetch_and_add(value, add)

#if ( __i386__ || __i386 || __amd64__ || __amd64 )
#	define cpu_pause()             __asm__ ("pause")
#else
#	define cpu_pause()
#endif

#define RWLOCK_SPIN 2048
#define RWLOCK_WLOCK  ((atomic_uint_t)-1)

template<typename T>
class ReadLocker
{
public:
    ReadLocker(T *lk):lk_(lk)
    {
        lk_->RLock();
    }
    ~ReadLocker()
    {
        lk_->UnLock();
    }
private:
    T * lk_;
};

template<typename T>
class WriteLocker
{
public:
    WriteLocker(T *lk):lk_(lk)
    {
        lk_->WLock();
    }
    ~WriteLocker()
    {
        lk_->UnLock();
    }
private:
    T * lk_;
};

class SharedRWLock
{
private:
	typedef long atomic_int_t;
	typedef unsigned long atomic_uint_t;
	typedef volatile atomic_uint_t  atomic_t;
	
	atomic_t * lock;
public:
	SharedRWLock(SharedStackAllocator & alloctor)
	{
		lock = alloctor.Push<atomic_t>();
		*lock = 0;
    }

	void WLock()
	{
		for (;;) 
		{
			if (*lock == 0 && atomic_cmp_set(lock, 0, RWLOCK_WLOCK))
				return;
			for (int n = 1; n < RWLOCK_SPIN; n <<= 1)
			{
				for (int i = 0; i < n; ++i)
					cpu_pause();
				if (*lock == 0 && atomic_cmp_set(lock, 0, RWLOCK_WLOCK))
					return;
			}
			sched_yield();
		}
    }

	void RLock()
	{
		atomic_uint_t readers;
		for (;;)
		{
			readers = *lock;
			if (readers != RWLOCK_WLOCK && atomic_cmp_set(lock, readers, readers + 1))
				return;

			for (int n = 1; n < RWLOCK_SPIN; n <<= 1)
			{
				for (int i = 0; i < n; ++i)
					cpu_pause();
				readers = *lock;
				if (*lock != RWLOCK_WLOCK && atomic_cmp_set(lock, readers, readers + 1))
					return;
			}
			sched_yield();
		}
    }

	bool TryRLock()
	{
		atomic_uint_t readers;
		readers = *lock;
		if (readers != RWLOCK_WLOCK && atomic_cmp_set(lock, readers, readers + 1))
			return true;

		for (int n = 1; n < RWLOCK_SPIN; n <<= 1)
		{
			for (int i = 0; i < n; ++i)
				cpu_pause();
			readers = *lock;
			if (*lock != RWLOCK_WLOCK && atomic_cmp_set(lock, readers, readers + 1))
				return true;
		}
		return false;
    }

	void UnLock()
	{
		atomic_uint_t readers = *lock;
		if (readers == RWLOCK_WLOCK)
		{
			*lock = 0;
			return;
		}

		for (;;)
		{
			if (atomic_cmp_set(lock, readers, readers-1))
			{
				return;
			}
			readers = *lock;
		}
    }
};
