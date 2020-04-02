#ifndef __htk_type_traits_h__
#define __htk_type_traits_h__


namespace htk
{
    template <typename T, T val>
    struct integral_constant
    {
        using value_type = T;
        using type = integral_constant;
        static constexpr T value = val;
        constexpr operator value_type() const noexcept
        {
            return value;
        }

        constexpr value_type operator()() const noexcept
        {
            return value;
        }
    };

    template <bool v>
    using bool_constant = integral_constant<bool, v>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

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

    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <typename T>
    struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

    template <typename T>
    constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

    template <typename T>
    struct is_trivially_constructible : bool_constant<__is_trivially_constructible(T)> {};

    template <typename T>
    constexpr bool is_trivially_constructible_v = is_trivially_constructible<T>::value;

    template <typename T>
    struct is_trivial : bool_constant<is_trivially_copyable_v<T> && is_trivially_constructible_v<T>>{};

    template <typename T>
    constexpr bool is_trivial_v = is_trivial<T>::value;

    template<typename T, typename ...Args>
    struct is_constructible : bool_constant<__is_constructible(T, Args...)> {};

    template <typename T>
    struct is_move_constructible : is_constructible<T, T&&> {};

    template <typename T>
    constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
}

#endif //__htk_type_traits_h__