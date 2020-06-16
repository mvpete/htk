#ifndef __htk_chronograph_h__
#define __htk_chronograph_h__

#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include <htk/format.h>
#include <htk/lap.h>

namespace chronograph
{
    namespace detail
    {
        std::string format_name(const std::string &name, size_t value)
        {
            std::stringstream ss;
            ss << name << " / " << value;
            return ss.str();
        }

        static std::initializer_list<size_t> default_runs { 10, 100, 1000, 10000, 100000 };
    }

    // The DoNotOptimize(...) function can be used to prevent a value or
    // expression from being optimized away by the compiler. This function is
    // intended to add little to no overhead.
    // See: https://youtu.be/nXaxk27zwlk?t=2441
    // https://stackoverflow.com/questions/33975479/escape-and-clobber-equivalent-in-msvc
    template <class Tp>
    inline auto do_not_optimize(Tp const &value)
    {
        return reinterpret_cast<const char volatile &>(value);
    }

    template <typename Callable>
    lap measure(Callable &&fn)
    {
        lap l;
        start(l);
        fn();
        stop(l);
        return l;
    }

    template <typename Callable>
    void measure(session_run &run, Callable &&fn)
    {
        run.record(measure(std::forward<Callable>(fn)));
    }

    template <typename Callable>
    typename std::enable_if<!std::is_invocable_v<Callable, session_run &>>::type
    benchmark(session &s, const std::string &name, Callable &&fn, std::initializer_list<size_t> runs = detail::default_runs)
    {
        int group = s.next_group();
        for (const auto iterations : runs)
        {
            session_run r(s, name, group);
            for (size_t i = 0; i < iterations; ++i)
            {
                measure(r, std::forward<Callable>(fn));
            }
        }
    }

    template <typename Callable>
    typename std::enable_if<std::is_invocable_v<Callable, session_run &>>::type
    benchmark(session &s, const std::string &name, Callable &&fn, std::initializer_list<size_t> runs = detail::default_runs)
    {
        int group = s.next_group();
        for (const auto iterations : runs)
        {
            session_run r(s, name, group);
            for (size_t i = 0; i < iterations; ++i)
            {
                fn(r);
            }
        }
    }

    template <typename Callable>
    typename std::enable_if<std::is_invocable_v<Callable, session_run &, size_t>>::type
    benchmark(session &s, const std::string &name, std::initializer_list<size_t> sizes, Callable &&fn,  std::initializer_list<size_t> runs = detail::default_runs)
    {
        s.add_column("size");
        for (const auto size : sizes)
        {
            int group = s.next_group();
            for (const auto iterations : runs)
            {
                session_run r(s, name, group);
                r.add_column_data(size);
                for (size_t i = 0; i < iterations; ++i)
                {
                    fn(r,size);
                }
            }
        }
    }

    struct tag_csv_format
    {
    };

    
    struct tag_table_format
    {
    };


    template <typename Stream>
    void output(Stream &ss, const session &s, tag_table_format)
    {
        format_table(ss, s);
    }

    template<typename Stream>
    void output(Stream &ss, const session &s, tag_csv_format)
    {
        format_csv(ss, s);
    }

};

#endif // __htk_chronograph_h__