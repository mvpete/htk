#ifndef __iterator_h__
#define __iterator_h__

#include <htk/type_traits.h>
#include <htk/types.h>

namespace htk
{
#if 0
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : input_iterator_tag {};
    struct bidirectional_iterator_tag : forward_iterator_tag {};
    struct random_access_iterator_tag : bidirectional_iterator_tag {};
#else
#include <iterator>
    using input_iterator_tag = ::std::input_iterator_tag;
    using output_iterator_tag = ::std::output_iterator_tag;
    using forward_iterator_tag = ::std::forward_iterator_tag;
    using bidirectional_iterator_tag = ::std::bidirectional_iterator_tag;
    using random_access_iterator_tag = ::std::random_access_iterator_tag;
#endif

    template <typename, typename = void>
    struct iterator_traits
    {
    };

    // uses SFINAE to overload if iterator has the traits.
    //
    template <typename Iterator>
    struct iterator_traits<Iterator,
        void_t<
            typename Iterator::iterator_category,
            typename Iterator::difference_type,
            typename Iterator::value_type,
            typename Iterator::pointer,
            typename Iterator::reference>>
    {
        using iterator_category = typename Iterator::iterator_category;
        using difference_type = typename Iterator::difference_type;
        using value_type = typename Iterator::value_type;
        using pointer = typename Iterator::pointer;
        using reference = typename Iterator::reference;
    };

    template <typename T>
    struct iterator_traits<T *>
    {
        using iterator_category = random_access_iterator_tag;
        using difference_type = htk::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
    };

    template <typename T>
    struct iterator_traits<const T *>
    {
        using iterator_category = random_access_iterator_tag;
        using difference_type = htk::ptrdiff_t;
        using value_type = T;
        using pointer = const T *;
        using reference = const T &;
    };

    template <typename T, typename = void>
    struct has_iterator_cat : false_type
    {
    };

    template <typename T>
    struct has_iterator_cat<T, void_t<typename iterator_traits<T>::iterator_category>> : true_type
    {
    };

    template <typename T>
    constexpr bool has_iterator_cat_v = has_iterator_cat<T>::value;

    template <typename Iterator>
    constexpr bool is_iterator_v = has_iterator_cat_v<Iterator>;

    namespace detail
    {
        template <typename IteratorT>
        typename iterator_traits<IteratorT>::pointer undress(IteratorT &&t);
    }
};

#endif // __iterator_h_;
