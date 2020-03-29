#ifndef __co_task_h__
#define __co_task_h__

#include <htk/detail/event.h>
#include <htk/co_schedule.h>

#include <experimental/coroutine>
#include <future>
#include <iostream>
#include <variant>

namespace htk
{
	struct avoid_md {};

	template <typename T>
	class co_task
	{

	private:

		struct promise_base
		{
			promise_base()
			{
			}

			virtual ~promise_base() = default;

			std::experimental::suspend_always initial_suspend() const noexcept
			{
				return {};
			}

			std::experimental::suspend_always final_suspend() const noexcept
			{
				return {};
			}

			bool is_complete()
			{
				return event_.is_set();
			}

			void wait()
			{
				event_.wait();
			}

			scheduler::detail::event event_;
		};

		template <typename ValueT>
		class promise : public promise_base
		{
		public:
			using value_t = typename std::remove_reference<ValueT>::type;
		private:
			std::variant<value_t, std::exception_ptr> value_;
		public:

			co_task get_return_object()
			{
				return std::experimental::coroutine_handle<promise>::from_promise(*this);
			}

			template<typename VALUE, typename = std::enable_if_t<std::is_convertible_v<VALUE &&, T>>>
			void return_value(VALUE &&value)
			{
				value_ = std::forward<VALUE>(value);
				promise_base::event_.signal();
			}
						
			void unhandled_exception()
			{
				value_ = std::current_exception();
				promise_base::event_.signal();
			}

			const value_t &get() const
			{
				promise_base::event_.wait();
				if (std::holds_alternative<std::exception_ptr>(value_))
				{
					std::rethrow_exception(std::get<std::exception_ptr>(value_));
				}
				return std::get<value_t>(value_);
			}

		};

		template <>
		class promise<void> : public promise_base
		{
			std::exception_ptr exception_;

		public:
			co_task get_return_object()
			{
				return std::experimental::coroutine_handle<promise>::from_promise(*this);
			}

			void unhandled_exception() noexcept
			{
				exception_ = std::current_exception();
				promise_base::event_.signal();
			}

			void return_void() noexcept
			{
				promise_base::event_.signal();
			}

			void get() const
			{
				promise_base::event_.wait();
				if (exception_)
					std::rethrow_exception(exception_);
			}

		};

	public:

		using promise_type = promise<T>;
		using coro_handle = std::experimental::coroutine_handle<promise_type>;

		co_task()
		{
		}

		co_task(coro_handle h)
			:handle_(h)
		{
		}

		bool resume() const
		{
			if (!is_complete())
				handle_.resume();
			return is_complete();
		}

		bool is_complete() const
		{
			return handle_.promise().is_complete();
		}

		T get()
		{
			return handle_.promise().get();
		}

		void wait() const
		{
			handle_.promise().wait();
		}

	private:
		coro_handle handle_;
	};

	namespace detail
	{

		template <typename T>
		struct co_task_awaiter
		{
			co_task<T> &task;

			bool await_ready() const
			{
				return task.is_complete();
			}

			void await_suspend(std::experimental::coroutine_handle<co_task::promise_type> resume_cb)
			{
				htk::scheduler::this_scheduler::schedule_now([resume_cb, this]()
				{
					task.resume();
					resume_cb();
				});
			}

			T await_resume()
			{
				return task.get();
			}

		};

		template <typename T>
		auto operator co_await(co_task<T> &&task)
		{
			return co_task_awaiter<T>{ task };
		}
	}

	template <typename T>
	co_task<T> co_schedule(co_task<T> &&task)
	{
		scheduler::this_scheduler::schedule_now([task]()
		{
			task.resume();
		});
		return task;
	}

	template <typename Tag>
	co_task<void> co_schedule(co_task<void>&& task, Tag t = htk::avoid_md{})
	{
		scheduler::this_scheduler::schedule_now([task]()
		{
			task.resume();
		});
		return task;
	}

	template <typename Scheduler, typename FnT>
	auto co_schedule(Scheduler &scheduler, FnT &&fn)
	{
		struct schedule_now_awaiter
		{
			using result_t = std::invoke_result_t<FnT()>;
			Scheduler &scheduler;
			FnT fn;
			bool await_ready() const
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> resume_cb)
			{
				scheduler::detail::schedule_now(scheduler, [resume_cb, this]()
				{
					resume_cb();
				});
			}

			auto await_resume() {
				return fn();
			}
			
		};
		return schedule_now_awaiter{ scheduler, fn };
	}

	template <typename FnT>
	auto co_schedule(FnT &&fn)
	{
		return co_schedule(scheduler::this_scheduler::get(), std::forward<FnT>(fn));
	}

	template <typename Scheduler>
	auto co_schedule(Scheduler &scheduler)
	{
		struct schedule_now_awaiter
		{
			Scheduler &scheduler;
			bool await_ready() const
			{
				return false;
			}

			void await_suspend(std::experimental::coroutine_handle<> resume_cb)
			{
				scheduler::detail::schedule_now(scheduler, [resume_cb, this]()
				{
					resume_cb();
				});
			}

			void await_resume() {}
		};
		return schedule_now_awaiter{scheduler};
	}

	auto co_schedule()
	{
		return co_schedule(scheduler::this_scheduler::get());
	}




	//template<typename CallableT>
	//co_task<typename std::result_of<CallableT()>::type>  co_schedule_at(CallableT &&fn)
	//{
	//	this_scheduler::get().schedule_at(fn);
	//}

	//template<typename CallableT>
	//co_task<typename std::result_of<CallableT()>::type>  co_schedule_after(CallableT &&fn)
	//{
	//	this_scheduler::get().schedule_after(fn);
	//}

};

#endif // __coroutine_h__
