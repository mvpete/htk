#ifndef __iterator_h__
#define __iterator_h__

namespace htk
{
    struct input_iterator_tag {};

    struct output_iterator_tag {};

    struct forward_iterator_tag : input_iterator_tag {};

    struct bidirectional_iterator_tag : forward_iterator_tag {};

    struct random_access_iterator_tag : bidirectional_iterator_tag {};
};

#endif // __iterator_h_;
