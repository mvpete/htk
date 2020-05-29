#ifndef __lap_h__
#define __lap_h__

#include <htk/clock.h>

namespace chronograph
{
    struct lap
    {
        clock::time_point start;
        clock::time_point stop;

        clock::duration duration() const
        {
            return stop - start;
        }
    };

    void start(lap& l)
    {
        l.start = clock::now();
    }

    void stop(lap& l)
    {
        l.stop = clock::now();
    }
}

#endif // __lap_h__