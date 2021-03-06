#ifndef __htk_memory_h__
#define __htk_memory_h__

#include <htk/types.h>
#include <htk/utility.h>
#include <htk/exception.h>

#include <xmemory>

namespace htk
{
    /*
         Note the allocator originally implemented by Stepanov, has 
         different methods, than today's MSVC allocator.

         I'm going to choose a hybrid set of methods for my allocator, which 
         are the set currently listed on cpp reference.


         https://web.archive.org/web/20160731194945/http://www.stepanovpapers.com/butler.hpl.hp/stl/stl/DEFALLOC.H
         https://en.cppreference.com/w/cpp/memory/allocator
    */
    

    template <typename T>
    struct allocator
    {
        using value_type = typename htk::remove_cvref_t<T>;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;

        allocator() = default;
        allocator(const allocator&) {}
        ~allocator() = default;

        // address
        pointer address(reference x) const
        {
            return &x;
        }

        const_pointer address(const_reference x) const
        {
            return &x;
        }

        pointer allocate(size_t count)
        {
            T* temp = static_cast<T*>(::operator new(count*sizeof(T)));
            if (temp == nullptr)
            {
                throw bad_alloc();
            }
            return temp;
        }

        void deallocate(pointer p, size_type n)
        {
            ::operator delete(p, n * sizeof(T));
        }

        size_type max_size() const
        {
            return max(size_type(1), size_type(UINT_MAX / sizeof(T)));
        }

        template <typename ...Args>
        void construct(reference dest, Args&&... args)
        {
            ::new (static_cast<void*>(&dest)) T(htk::forward<Args>(args)...);
        }

        void destroy(pointer p)
        {
            p->~T();
        }
    };

    template <>
    struct allocator<void>
    {
        using pointer = void*;
    };

    template <typename T, typename U>
    bool operator==(const allocator<T> &l, const allocator<U> &r)
    {
        return true;
    }

    template <typename T, typename U>
    bool operator!=(const allocator<T>& l, const allocator<U>& r)
    {
        return !operator==(l,r);
    }
}


#endif // __htk_memory_h__