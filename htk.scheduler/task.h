#ifndef __task_h__
#define __task_h__

#include <future>
#include <memory>

namespace htk
{
    template <typename ClockT>
    class task
    {
    public:
        using ptr = std::shared_ptr<task>;

        virtual ~task() = default;
        virtual typename ClockT::duration time_from_now() const = 0;
        virtual bool again() const = 0;
        virtual void operator()() = 0;
    };

    template <typename ClockT>
    class task_comparator
    {
    public:
        
        bool operator()(const typename task<ClockT>::ptr &a, const typename task<ClockT>::ptr &b)
        {
            return a->time_from_now() > b->time_from_now();
        }
    };

}


#endif // __task_h__