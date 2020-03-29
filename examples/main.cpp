
#include <htk/co_routine.h>

#include <iostream>
#include <future>

htk::co_task<int> r()
{
	//co_await htk::co_schedule();
	co_return 4;
}


htk::co_task<int> r3()
{
	auto val = co_await htk::co_schedule([]() { return 6; });
	co_return val;
}

htk::co_task<void> resume_background()
{
	std::cout << std::this_thread::get_id() << ": " << "This happens." << std::endl;
	co_await std::experimental::suspend_always{}; // suspension point, returns control back to caller
	std::cout << std::this_thread::get_id() << ": " << "Then this happens." << std::endl;
}
//
//htk::co_task<int> something_int_async()
//{
//	std::cout << std::this_thread::get_id() << ": " << "This on this thread." << std::endl;
//	co_await htk::resume_now();
//	co_return 4;
//}
//
//htk::co_task<void> something_async()
//{
//    std::cout << std::this_thread::get_id() << ": " << "This...." << std::endl;
//    co_await htk::resume_now();
//    std::cout << std::this_thread::get_id() << ": " << "And then this..." << std::endl;
//}
//
//htk::co_task<void> nested_async()
//{
//    std::cout << std::this_thread::get_id() << ": " << "Start your horses" << std::endl;
//    co_await something_async();
//	std::cout << std::this_thread::get_id() << ": " << "Then the next one" << std::endl;
//    co_await something_async();
//}

int main()
{
	std::cout << "main thread: " << std::this_thread::get_id() << std::endl;
	//auto task = htk::co_schedule(nested_async());
	htk::co_task<int> task_2 = htk::co_schedule(r());

	auto task_3 = htk::co_schedule(r3());
    //std::cout << "Initial resume" << std::endl;

	/*std::thread t([]()
	{
		std::cout << "thread 1: " << std::this_thread::get_id() << std::endl;
		htk::scheduler::this_scheduler::get().run();
	});

	std::thread t2([]()
	{
		std::cout << "thread 2: " << std::this_thread::get_id() << std::endl;
		htk::scheduler::this_scheduler::get().run();
	});*/

	try
	{
		std::cout << "task_2::get() : " << task_2.get() << std::endl;
		std::cout << "task_3::get() : " << task_3.get() << std::endl;
		htk::scheduler::this_scheduler::get().stop();
		/*t.join();
		t2.join();*/
	}
	catch (const std::exception & e)
	{
		std::cout << std::this_thread::get_id() << ": " << "an exception occurred: " << e.what() << std::endl;
	}
    return 0;
}