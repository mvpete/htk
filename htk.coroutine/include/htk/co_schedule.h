#ifndef __co_schedule_h__
#define __co_schedule_h__

#include <htk/detail/scheduler.h>
#include <htk/detail/task.h>

namespace htk
{
	namespace scheduler
	{
		using steady_scheduler = detail::steady_scheduler;

		namespace detail
		{
			struct static_scheduler
			{
				static steady_scheduler& get()
				{
					static detail::steady_scheduler instance;
					return instance;
				}
			};

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
			
			// to avoid mulitple definitions, create a struct with a static get,
			// then inline the global method.
			inline steady_scheduler& get()
			{
				return detail::static_scheduler::get();
			}
			

			template <typename FnT>
			void schedule_now(FnT &&fn)
			{
				scheduler::detail::schedule_now(this_scheduler::get(), std::forward<FnT>(fn));
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