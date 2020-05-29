#ifndef __format_h__
#define __format_h__

#include <htk/session.h>
#include <htk/statistics.h>
#include <iostream>
#include <iomanip>

namespace chronograph
{
    struct fill
    {
        fill(char c, int width)
            :c(c), w(width)
        {
        }
        char c;
        int w;
    };

    template <typename T>
    struct duration_traits
    {
    };

    template <>
    struct duration_traits<std::chrono::nanoseconds>
    {
        using type = std::chrono::nanoseconds;
        static constexpr const char *unit = "ns";
    };

    template <>
    struct duration_traits<std::chrono::microseconds>
    {
        using type = std::chrono::microseconds;
        static constexpr const char * unit = "us";
    };


    template <>
    struct duration_traits<std::chrono::milliseconds>
    {
        using type = std::chrono::milliseconds;
        static constexpr const char *unit = "ms";
    };

    template <typename Stream>
    Stream& operator<<(Stream& s, const fill& f)
    {
        for (size_t i = 0; i < f.w; ++i)
            s << f.c;
        return s;
    }

    template <typename Stream>
    void format_table(Stream& ss, const session& ses)
    {       
        using dtraits = duration_traits<std::chrono::nanoseconds>; 

        size_t max_width = 0;
        for (const auto& run : ses.runs())
            max_width = std::max(run.name.length(), max_width);
        max_width += 3;
        ss << fill('-', max_width+50) <<std::endl;
        ss  << std::left << std::setw(max_width) << std::setfill(' ') << "name"
             << std::right << std::setw(10) << std::setfill(' ') << "total" 
             << std::right << std::setw(10) << std::setfill(' ') << "mean" 
             << std::right << std::setw(10) << std::setfill(' ') << "median" << "\r\n";
        ss << fill('-', max_width+50) << std::endl;

        std::pair<bool, int> group;
        for (const auto& run : ses.runs())
        {
            if (!group.first)
            {
                group.first = true;
                group.second = run.group;
            }
            else if (group.second != run.group)
            {
                std::cout << "\r\n";
                group.second = run.group;
            }
            const auto stats = calculate_statistics<dtraits::type>(run);
            ss  << std::left << std::setw(max_width) << std::setfill(' ')  << run.name
               << std::right << std::setw(10) << std::setfill(' ') << std::chrono::duration_cast<dtraits::type>(run.total.duration()).count() << " " << dtraits::unit << " "
               << std::right << std::setw(10) << std::setfill(' ') << stats.mean << " " << dtraits::unit << " "
               << std::right << std::setw(10) << std::setfill(' ') << stats.median << " " << dtraits::unit << " " << "\r\n";
        }
          
    }
};

#endif // __format_h__
