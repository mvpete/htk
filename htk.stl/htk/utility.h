#ifndef __htk_utility_h__
#define __htk_utility_h__

#include <htk/type_traits.h>

namespace htk
{

    size_t min(size_t a, size_t b)
    {
        return a < b ? a : b;
    }

    size_t max(size_t a, size_t b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    constexpr T&& forward(remove_reference_t<T>& t)
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(remove_reference_t<T>&& t)
    {
        return static_cast<T&&>(t);
    }


    template <typename T>
    constexpr auto&& move(T&& t)
    {
        return static_cast<remove_reference_t<T>&&>(t);
    }


    template<bool B, class T, class F>
    struct conditional { typedef T type; };

    template<class T, class F>
    struct conditional<false, T, F> { typedef F type; };
}

#endif // __htk_utility_h__