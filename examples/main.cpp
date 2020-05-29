
#include <chrono>
#include <htk/chronograph.h>
#include <iostream>
#include <random>
#include <thread>

#include <htk/vector.h>
#include <vector>

using namespace chronograph;

template <typename T, typename VectorT>
VectorT random_numeric_vector(size_t size)
{
    VectorT v;

    std::default_random_engine generator;
    std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

    for (size_t i = 0; i < size; ++i)
    {
        v.emplace_back(distribution(generator));
    }
    return v;
}

size_t next_random(size_t max)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<size_t> distribution(0, max);
    return distribution(generator);
}


int main()
{
    session s;
    {
        std::vector<int> v;
        benchmark(s, "vector<int> emplace 10", [&v]() {
            for (int i = 0; i < 10; ++i)
                v.emplace_back(10);
        });
    }
    {
        htk::vector<int> hv;
        benchmark(s, "htk::vector<int> emplace 10", [&hv]() {
            for (int i = 0; i < 10; ++i)
                hv.emplace_back(10);
        });
    }

    {
        std::vector<int> v;
        benchmark(s, "vector<int> emplace 100", [&v]() {
            for (int i = 0; i < 100; ++i)
                v.emplace_back(10);
        });
    }
    {
        htk::vector<int> hv;
        benchmark(s, "htk::vector<int> emplace 100", [&hv]() {
            for (int i = 0; i < 100; ++i)
                hv.emplace_back(i);
        });
    }

    {
        std::vector<int> v;
        benchmark(s, "vector<int> emplace 1000", [&v]() {
            for (int i = 0; i < 1000; ++i)
                v.emplace_back(10);
        });
    }
    {
        htk::vector<int> hv;
        benchmark(s, "htk::vector<int> emplace 10000", [&hv]() {
            for (int i = 0; i < 1000; ++i)
                hv.emplace_back(10);
        });
    }

    {
        std::vector<int> v;
        benchmark(s, "vector<int> emplace 10000", [&v]() {
            for (int i = 0; i < 10000; ++i)
                v.emplace_back(10);
        });
    }
    {
        htk::vector<int> hv;
        benchmark(s, "htk::vector<int> emplace 10000", [&hv]() {
            for (int i = 0; i < 10000; ++i)
                hv.emplace_back(10);
        });
    }

    {
        benchmark(s, "vector<int> insert-end", [](session_run &r) {
            std::vector<int> v = random_numeric_vector<int, std::vector<int>>(1000);
            std::vector<int> v2 = random_numeric_vector<int, std::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.end(), v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "htk::vector<int> insert-end", [](session_run &r) {
            auto v = random_numeric_vector<int, htk::vector<int>>(1000);
            auto v2 = random_numeric_vector<int, htk::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.end(), v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "vector<int> insert-begin", [](session_run &r) {
            std::vector<int> v = random_numeric_vector<int, std::vector<int>>(1000);
            std::vector<int> v2 = random_numeric_vector<int, std::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.begin(), v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "htk::vector<int> insert-begin", [](session_run &r) {
            auto v = random_numeric_vector<int, htk::vector<int>>(1000);
            auto v2 = random_numeric_vector<int, htk::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.begin(), v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "vector<int> insert-mid", [](session_run &r) {
            std::vector<int> v = random_numeric_vector<int, std::vector<int>>(1000);
            std::vector<int> v2 = random_numeric_vector<int, std::vector<int>>(1000);
            auto i = next_random(v.size()-1);
            auto insert = v.begin() + i;
            measure(r, [&v, &v2, insert]() {
                v.insert(insert, v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "htk::vector<int> insert-mid", [](session_run &r) {
            auto v = random_numeric_vector<int, htk::vector<int>>(1000);
            auto v2 = random_numeric_vector<int, htk::vector<int>>(1000);
            auto i = next_random(v.size() - 1);
            auto insert = v.begin() + i;
            measure(r, [&v, &v2, insert]() {
                v.insert(insert, v2.begin(), v2.end());
            });
        });
    }


    format_table(std::cout, s);
}