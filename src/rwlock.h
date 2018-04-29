#pragma once
//#include <pthread.h>
#if 0
class RWLock
{
public:
	RWLock()
	{
		pthread_rwlock_init(&lock_, NULL);
	};
	~RWLock()
	{
		pthread_rwlock_destroy(&lock_);
	};
	void ReadLock()
	{
		pthread_rwlock_rdlock(&lock_);
	};
	void WriteLock()
	{
		pthread_rwlock_wrlock(&lock_);
	};
	void UnLock()
	{
		pthread_rwlock_unlock(&lock_);
	};
private:
	pthread_rwlock_t lock_;
};
#endif

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
