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
    
    template<bool B, typename T, typename F>
    struct conditional 
    { 
        using type = T; 
    };

    template<typename T, typename F>
    struct conditional<false, T, F> 
    { 
        using type = F; 
    };

    template <bool Eval, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> 
    {
        using type = T;
    };

    template <bool Eval, typename T = void>
    using enable_if_t = typename enable_if<Eval, T>::type;

}

#endif // __htk_utility_h__