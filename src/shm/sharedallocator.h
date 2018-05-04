#ifndef SHAREDALLOCATOR_H
#define SHAREDALLOCATOR_H

template<typename _Tp>
class SharedAllocator
{
public:
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;
    typedef _Tp*       pointer;
    typedef const _Tp* const_pointer;
    typedef _Tp&       reference;
    typedef const _Tp& const_reference;
    typedef _Tp        value_type;

    template<typename _Tp1>
    struct rebind
    { typedef SharedAllocator<_Tp1> other; };

    SharedAllocator() { }
    SharedAllocator(const SharedAllocator&) { }
    template<typename _Tp1>
    SharedAllocator(const SharedAllocator<_Tp1>&) { }
    ~SharedAllocator() { }

    pointer address(reference __x) const
    { return std::__addressof(__x); }

    const_pointer address(const_reference __x) const
    { return std::__addressof(__x); }

    pointer allocate(size_type __n, const void* = 0)
    {
        return Hund::Slabs::I().Malloc<_Tp>(__n);
    }

    void deallocate(pointer __p, size_type)
    {
        Hund::Slabs::I().Free(__p);
    };

    void construct(pointer __p, const _Tp& __val)
    { ::new((void *)__p) _Tp(__val); }

    void destroy(pointer __p)
    { __p->~_Tp(); }

    size_type max_size() const
    { return size_t(-1) / sizeof(_Tp); }
};


#endif // SHAREDALLOCATOR_H
