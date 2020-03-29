#ifndef __htk_type_traits_h__
#define __htk_type_traits_h__

namespace htk
{
    template <typename T> 
    struct remove_reference 
    { 
        using type = T;
    };

    // specialization for lvalue reference
    template <typename T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    // specialization for rvalue reference
    template <typename T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;
}

#endif //__htk_type_traits_h__