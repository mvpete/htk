
#include "co_task.h"
#include <iostream>



htk::co_task something_async()
{
    std::cout << std::this_thread::get_id() << ": " << "This...." << std::endl;
    co_await std::experimental::suspend_always{};
    std::cout << std::this_thread::get_id() << ": " << "And then this..." << std::endl;
}

htk::co_task nested_async()
{
    std::cout << std::this_thread::get_id() << ": " << "Start your horses" << std::endl;
    co_await something_async();
    std::cout << std::this_thread::get_id() << ": " << "I awaited" << std::endl;
    co_await something_async();
}

int main()
{
    auto task = nested_async();
    //std::cout << "Initial resume" << std::endl;
    //task.resume();
    task.wait();
    return 0;
}