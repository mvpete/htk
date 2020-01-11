#ifndef __co_event_h__
#define __co_event_h__

#include <condition_variable>
#include <mutex>
#include <chrono>

namespace htk
{
	namespace scheduler
	{
		namespace detail
		{
			template <typename ClockT>
			class event_t
			{
			public:

				event_t()
					:set_(false)
				{
				}

				void wait() const
				{
					std::unique_lock<std::mutex> lk(mtx_);
					cv_.wait(lk, [this]()
					{
						return set_;
					});
				}

				bool wait_for(typename ClockT::duration d)
				{
					std::unique_lock<std::mutex> lk(mtx_);
					auto val = cv_.wait_for(lk, d, [this]()
					{
						return set_;
					});
					return val;
				}

				bool wait_until(typename ClockT::time_point t)
				{
					std::unique_lock<std::mutex> lk(mtx_);
					auto val = cv_.wait_until(lk, t, [this]()
					{
						return set_;
					});
					return val;
				}

				void signal()
				{
					set_ = true;
					cv_.notify_one();
				}

				void signal_all()
				{
					set_ = true;
					cv_.notify_all();
				}

				bool is_set() const
				{
					return set_;
				}

				void reset()
				{
					set_ = false;
				}

			private:

				mutable std::mutex mtx_;
				mutable std::condition_variable cv_;
				mutable bool set_;
			};

			using event = event_t<std::chrono::steady_clock>;
		}
	}

};

#endif // _co_event_h__
