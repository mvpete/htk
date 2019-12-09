#ifndef __steady_task_h__
#define __steady_task_h__

#include <chrono>
#include <functional>
#include <task.h>

namespace htk
{
    template <typename ClockT>
    class duration_task : public task<ClockT>
    {
    public:
        template <typename CallableT>
        duration_task(typename ClockT::duration d, CallableT &&fn)
            :last_(typename ClockT::now()), interval_(d), fn_(std::forward<CallableT>(fn))
        {

        }

        std::chrono::steady_clock::duration time_from_now() const
        {
            return (last_ + interval_) - typename ClockT::now();
        }
        
        virtual bool again() const
        {
            return false;
        }
        
        virtual void operator()()
        {
            fn_();
        }

    private:
        typename ClockT::time_point last_;
        typename ClockT::duration interval_;
        std::function<void()> fn_;

    };

    using steady_task = duration_task<std::chrono::steady_clock>;
    using system_task = duration_task<std::chrono::system_clock>;
}

#endif //__steady_task_h__
