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
    benchmark(session &s, const std::string &name, Callable &&fn, std::initializer_list<size_t> runs = { 10, 100, 1000 })
    {
        // create the session
        // follow some warmup
        // I want to measure each iteration, and the total time.
        int group = s.next_group();
        for (const auto iterations : runs)
        {
            session_run r(s, detail::format_name(name, iterations), group);
            for (size_t i = 0; i < iterations; ++i)
            {
                measure(r, std::forward<Callable>(fn));
            }
        }
    }

    template <typename Callable>
    typename std::enable_if<std::is_invocable_v<Callable, session_run &>>::type
    benchmark(session &s, const std::string &name, Callable &&fn, std::initializer_list<size_t> runs = { 10, 100, 1000 })
    {
        int group = s.next_group();
        for (const auto iterations : runs)
        {
            session_run r(s, detail::format_name(name, iterations), group);
            for (size_t i = 0; i < iterations; ++i)
            {
                fn(r);
            }
        }
    }

    template <typename Callable>
    typename std::enable_if<std::is_invocable_v<Callable, session &, size_t>>::type
    benchmark(session &s, const std::string &name, Callable &&fn, std::initializer_list<size_t> runs = { 10, 100, 1000 })
    {
        int group = s.next_group();
        for (const auto iterations : runs)
        {
            fn(s, iterations, group);
        }
    }

};

#endif // __htk_chronograph_h__