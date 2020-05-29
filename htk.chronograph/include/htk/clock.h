#ifndef __clock_h__
#define __clock_h__

#include <chrono>
namespace chronograph
{
    struct clock
    {
        using type = std::chrono::high_resolution_clock;
        using time_point = type::time_point;
        using duration = type::duration;

        static time_point now()
        {
            return type::now();
        }
    };
};

#endif // __clock_h__