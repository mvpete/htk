#ifndef __algorithm_h__
#define __algorithm_h__

namespace htk
{
    template <typename IteratorT, typename ValueT>
    bool binary_search(IteratorT begin, IteratorT end, const ValueT &v)
    {
        const int mid = (end - begin) / 2;
        if (mid == 0)
            return *begin == v ? true : ( *(end-1) == v ? true  : false);
        const auto midval = *(begin + mid);
        if (midval == v)
            return true;
        else if (v < midval)
            return htk::binary_search(begin, begin+mid, v);
        else
            return htk::binary_search(begin + mid, end, v);
    }
    template <typename IteratorT, typename ValueT>
    bool linear_search(IteratorT begin, IteratorT end, const ValueT &v)
    {
        for (; begin != end; ++begin)
        {
            if (*begin == v)
                return true;
        }
        return false;
    }
}





#endif //__algorithm_h__
