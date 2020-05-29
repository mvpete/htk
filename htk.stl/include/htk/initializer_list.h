#ifndef __htk_initializer_list_h__
#define __htk_initializer_list_h__

#include <htk/types.h>

namespace htk
{

#if 1
#include <initializer_list>
    // std::initializer_list seems to be basically a compiler intrinsic
    // so if you write your own, it doesn't matter
    // Therefor, we'll just alias it. So it looks like were
    // keeping it in htk namespace
    template <typename T>
    using initializer_list = std::initializer_list<T>;
#else
    template <typename T>
    class initializer_list
    {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using size_type = htk::size_t;
        using iterator = const T*;
        using const_iterator = const T*;

    public:
        initializer_list()
            :begin_(nullptr), end_(nullptr)
        {
        }

        initializer_list(const T* begin, const T* end)
            :begin_(begin), end_(end)
        {
        }

    public:

        iterator begin() { return begin_;  }
        iterator end() { return end_; }

        const_iterator begin() const { return begin_; }
        const_iterator end() const { return end_; }

    private:

        const T* const begin_;
        const T* const end_;
    };


    template <typename T>
    typename initializer_list<T>::iterator begin(const initializer_list<T>& it)
    {
        return it.begin();
    }

    template <typename T>
    typename initializer_list<T>::iterator end(const initializer_list<T>& it)
    {
        return it.end();
    }
#endif
}

#endif // __htk_initializer_list_h__