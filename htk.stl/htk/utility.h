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
    constexpr T&& forward(remove_reference_t<T>& arg)
    {
        return static_cast<T&&>(arg);
    }

    template <typename T>
    constexpr T&& forward(remove_reference_t<T>&& arg)
    {
        return static_cast<T&&>(arg);
    }

}

#endif // __htk_utility_h__