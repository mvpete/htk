#ifndef __scheduler_h__
#define __scheduler_h__

#include <detail/event.h>
#include <future>
#include <queue>
#include <task.h>
#include <vector>

namespace htk 
{
namespace detail
{
	template <typename ClockT>
	class scheduler
	{
	public:
		scheduler()
			:stopped_(false)
		{
		}

		void run()
		{
			while (run_one());
		}

		bool run_one()
		{
			std::unique_lock<std::mutex> lk(qmtx_);
			if (stopped_)
				return false;

			while (tasks_.empty())
			{
				lk.unlock();
				interrupt_.sleep();
				lk.lock();
			}

			while (!stopped_)
			{
				auto task = tasks_.top();
				typename ClockT::duration ttl = task->time_from_now();
				if (ttl <= std::chrono::seconds::zero())
				{
					// I own a task now, give back the conch            
					tasks_.pop();
					lk.unlock();
					// execute it.
					(*task)();
					if (task->again())
					{
						// will re-claim mtx
						schedule_task(task);
					}
					return true;
				}
				else
				{
					lk.unlock();
					interrupt_.wait_for(ttl);
					lk.lock();
				}
			}
			return false;

		}

		void stop()
		{
			std::unique_lock<std::mutex> lk(qmtx_);
			stopped_ = true;
			interrupt_.wake_all();
		}


		template <typename CallableT>
		void schedule_now(CallableT &&fn)
		{
		}

		template <typename CallableT>
		void schedule_at(CallableT &&fn, ...)
		{
		}

		template <typename CallableT>
		void schedule_after(CallableT &&fn, ...)
		{
		}

	private:

		void schedule_task(typename task<ClockT>::ptr t)
		{
			std::unique_lock<std::mutex> lk(qmtx_);
			if (stopped_)
				return;
			tasks_.emplace(std::move(t));
			interrupt_.wake();
		}

		event_t<ClockT> interrupt_;
		std::mutex qmtx_;
		std::priority_queue<typename task<ClockT>::ptr, std::vector<typename task<ClockT>::ptr>, typename task_comparator<ClockT>> tasks_;
		bool stopped_;
	};

	using system_scheduler = scheduler<std::chrono::system_clock>;
	using steady_scheduler = scheduler<std::chrono::steady_clock>;
	
	namespace this_scheduler
	{
		steady_scheduler &get()
		{
			static steady_scheduler instance;
			return instance;
		}
	}

}
}


#endif // __scheduler_h__
