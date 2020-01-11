#ifndef __co_schedule_h__
#define __co_schedule_h__

#include <detail/scheduler.h>
#include <detail/task.h>

namespace htk
{
	namespace scheduler
	{
		namespace detail
		{
			template <typename Scheduler, typename Function>
			void schedule_now(Scheduler &scheduler, Function &&fn)
			{
				scheduler.schedule_task(std::make_shared<detail::steady_now_task>(fn));
			}

			template <typename Scheduler, typename Function>
			void schedule_after(Scheduler &scheduler, Function &&fn, const std::chrono::steady_clock::duration &duration)
			{
				scheduler.schedule_task(std::make_shared<detail::steady_after_task>(fn, duration));
			}

			template <typename Scheduler, typename Function>
			void schedule_at(Scheduler &scheduler, Function &&fn)
			{
			}
		}

		namespace this_scheduler
		{
			detail::steady_scheduler &get()
			{
				static detail::steady_scheduler instance;
				return instance;
			}

			template <typename FnT>
			void schedule_now(FnT &&fn)
			{
				scheduler::detail::schedule_now(get(), std::forward<FnT>(fn));
			}

			template <typename FnT>
			void schedule_at(FnT &&fn)
			{
				// ...
			}

			template <typename FnT>
			void schedule_after(FnT &&fn)
			{
				// ...
			}


		}
	}

	
};

#endif //__co_schedule_h__