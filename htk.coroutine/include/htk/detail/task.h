#ifndef __task_h__
#define __task_h__

#include <future>
#include <memory>

namespace htk
{
	namespace scheduler
	{
		namespace detail
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

			template <typename ClockT>
			class now_task : public task<ClockT>
			{
			public:
				template <typename CallableT>
				now_task(CallableT &&fn)
					:fn_(std::forward<CallableT>(fn))
				{

				}

				typename ClockT::duration time_from_now() const
				{
					return ClockT::duration::zero();
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
				std::function<void()> fn_;

			};

			using steady_now_task = now_task<std::chrono::steady_clock>;
			using system_now_task = now_task<std::chrono::system_clock>;

			template <typename ClockT>
			class after_task : public task<ClockT>
			{
			public:
				template <typename CallableT>
				after_task(typename ClockT::duration d, CallableT &&fn)
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

			using steady_after_task = after_task<std::chrono::steady_clock>;
			using system_after_task = after_task<std::chrono::system_clock>;


		}
	}   
}


#endif // __task_h__