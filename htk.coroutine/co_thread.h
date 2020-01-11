#ifndef __co_background_h__
#define __co_background_h__

#include <experimental/coroutine>
#include <thread>

/*
	An awaiter that resumes the co-routine on a background thread.
*/

namespace htk
{
	namespace detail
	{
		struct thread_awaiter
	{
		bool await_ready() const
		{
			// not ready, forces suspension
			return false;
		}

		void await_suspend(std::experimental::coroutine_handle<> resume_cb)
		{
			std::thread t([resume_cb, this]() {
				resume_cb();
			});
			t.detach();
		}

		void await_resume()
		{
		}
	};
	}

	auto schedule_on_thread()
	{
		return detail::thread_awaiter{};
	}
};

#endif // __co_background_h__
