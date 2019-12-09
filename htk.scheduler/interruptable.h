#ifndef __interruptable_h__
#define __interruptable_h__

#include <mutex>
#include <condition_variable>

namespace htk
{
    template <typename ClockT>
    class interruptable
    {
    public:
        interruptable()
            :wake_(false)
        {
        }

        bool sleep_for(typename ClockT::duration d)
        {
            std::unique_lock<std::mutex> lk(mtx_);
            auto val = cv_.wait_for(lk, d, [this]()
            {
                return wake_;
            });
            wake_ = false;
            return val;
        }

        bool sleep_until(typename ClockT::time_point t)
        {
            std::unique_lock<std::mutex> lk(mtx_);
            auto val = cv_.wait_until(lk, t, [this]()
            {
                return wake_;
            });
            wake_ = false;
            return val;
        }

        void sleep()
        {
            std::unique_lock<std::mutex> lk(mtx_);
            cv_.wait(lk, [this]()
            {
                return wake_;
            });
            wake_ = false;
        }

        void wake()
        {
            wake_ = true;
            cv_.notify_one();
        }

        void wake_all()
        {
            wake_ = true;
            cv_.notify_all();
        }

    private:
        std::mutex mtx_;
        std::condition_variable cv_;
        bool wake_;
    };


}


#endif //__interruptable_h__