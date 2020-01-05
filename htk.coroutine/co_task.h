#ifndef __co_task_h__
#define __co_task_h__

#include <experimental/coroutine>
#include <future>
#include <iostream>

namespace htk
{

    class co_task
    {
    public:
        struct promise_type;
        using promise_t = promise_type;
        using coro_handle = std::experimental::coroutine_handle<typename promise_t>;

        co_task(coro_handle h)
            :handle_(h)
        {
            std::cout << std::this_thread::get_id() << " : co_task()" << std::endl;
        }

        bool resume()
        {
            std::cout << std::this_thread::get_id() << " : resume()" << std::endl;
            if (!done())
                handle_.resume();
            return done();
        }

        bool done() const
        {
            return handle_.done();
        }

        void wait()
        {
            while (!done());
        }

        struct promise_type
        {

            co_task get_return_object()
            {
                std::cout << std::this_thread::get_id() << " : get_return_object" << std::endl;
                return coro_handle::from_promise(*this);
            }
            std::experimental::suspend_always initial_suspend() const noexcept
            {
                std::cout << std::this_thread::get_id() << " : initial_suspend" << std::endl;
                return {};
            }
            std::experimental::suspend_always final_suspend() const noexcept
            {
                std::cout << std::this_thread::get_id() << " : final_suspend" << std::endl;
                return {};
            }
            void unhandled_exception() const noexcept
            {
                std::terminate();
            }
            void return_void() const noexcept
            {
                std::cout << std::this_thread::get_id() << " : return_void" << std::endl;
            }
            
        };


    private:
        coro_handle handle_;
    };

    struct co_task_awaiter
    {
        co_task& task;

        bool await_ready() const
        {
            std::cout << std::this_thread::get_id()  << " : await_ready" << std::endl;
            return task.done();
        }

        void await_suspend(std::experimental::coroutine_handle<> resume_cb)
        {
            std::cout << std::this_thread::get_id()  << " : await_suspend" << std::endl;
            std::thread t([resume_cb, this]() { 
            
                std::cout << std::this_thread::get_id() << " : in thread" << std::endl;
                resume_cb(); 
            });
            t.detach();
        }

        decltype(auto) await_resume()
        {
            std::cout << std::this_thread::get_id() << " : await_resume" << std::endl;
            task.resume();
        }

    };

    auto operator co_await(co_task&& task)
    {
        return co_task_awaiter{ task };
    }


    //namespace experimental {
    //    template <class _Ty, class... _ArgTypes>
    //    struct coroutine_traits<future<_Ty>, _ArgTypes...> {
    //        // defines resumable traits for functions returning future<_Ty>
    //        struct promise_type {
    //            promise<_Ty> _MyPromise;

    //            future<_Ty> get_return_object() {
    //                return _MyPromise.get_future();
    //            }

    //            suspend_never initial_suspend() const noexcept {
    //                return {};
    //            }

    //            suspend_never final_suspend() const noexcept {
    //                return {};
    //            }

    //            template <class _Ut>
    //            void return_value(_Ut&& _Value) {
    //                _MyPromise.set_value(_STD forward<_Ut>(_Value));
    //            }

    //            void unhandled_exception() {
    //                _MyPromise.set_exception(_STD current_exception());
    //            }
    //        };
    //    };

    //    template <class... _ArgTypes>
    //    struct coroutine_traits<future<void>, _ArgTypes...> {
    //        // defines resumable traits for functions returning future<void>
    //        struct promise_type {
    //            promise<void> _MyPromise;

    //            future<void> get_return_object() {
    //                return _MyPromise.get_future();
    //            }

    //            suspend_never initial_suspend() const noexcept {
    //                return {};
    //            }

    //            suspend_never final_suspend() const noexcept {
    //                return {};
    //            }

    //            void return_void() {
    //                _MyPromise.set_value();
    //            }

    //            void unhandled_exception() {
    //                _MyPromise.set_exception(_STD current_exception());
    //            }
    //        };
    //    };

    //    template <class _Ty>
    //    struct _Future_awaiter {
    //        future<_Ty>& _Fut;

    //        bool await_ready() const {
    //            return _Fut._Is_ready();
    //        }

    //        void await_suspend(experimental::coroutine_handle<> _ResumeCb) {
    //            // TRANSITION, change to .then if and when future gets .then
    //            thread _WaitingThread([&_Fut = _Fut, _ResumeCb]() mutable {
    //                _Fut.wait();
    //                _ResumeCb();
    //                });
    //            _WaitingThread.detach();
    //        }

    //        decltype(auto) await_resume() {
    //            return _Fut.get();
    //        }
    //    };

    //} // namespace experimental


};

#endif // __coroutine_h__
