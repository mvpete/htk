
#include <chrono>
#include <fstream>
#include <htk/chronograph.h>
#include <iostream>
#include <random>
#include <thread>

#include <htk/algorithm.h>
#include <htk/vector.h>
#include <vector>

using namespace chronograph;

static std::default_random_engine generator;

struct move_only
{
    move_only(int value)
        : value(value)
    {
    }

    move_only(const move_only &) = delete;
    move_only(move_only &&m) noexcept
        : value(0)
    {
        std::swap(value, m.value);
    }

    int value;
};

template <typename T, typename VectorT>
VectorT random_numeric_vector(size_t size, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max())
{
    VectorT v;

    std::uniform_int_distribution<T> distribution(min, max);

    for (size_t i = 0; i < size; ++i)
    {
        v.emplace_back(distribution(generator));
    }
    return v;
}

size_t next_random(size_t min, size_t max)
{
    std::uniform_int_distribution<size_t> distribution(min, max);
    return distribution(generator);
}

size_t next_random(size_t max)
{
    return next_random(0, max);
}



void measure_vector_int_emplace(session &s)
{
    {
        benchmark(s, "vector<int> emplace 10", [](session_run &r) {
            std::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<int> emplace 10", [](session_run &r) {
            htk::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<int> emplace 100", [](session_run &r) {
            std::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<int> emplace 100", [](session_run &r) {
            htk::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<int> emplace 1000", [](session_run &r) {
            std::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 1000; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<int> emplace 1000", [](session_run &r) {
            htk::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 1000; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<int> emplace 10000", [](session_run &r) {
            std::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10000; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<int> emplace 10000", [](session_run &r) {
            htk::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10000; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<int> emplace 100000", [](session_run &r) {
            std::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100000; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<int> emplace 100000", [](session_run &r) {
            htk::vector<int> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100000; ++i)
                    v.emplace_back(10);
            });
        });
    }
}

void measure_vector_move_only_emplace(session &s)
{
    {
        benchmark(s, "vector<move_only> emplace 10", [](session_run &r) {
            std::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<move_only> emplace 10", [](session_run &r) {
            htk::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<move_only> emplace 100", [](session_run &r) {
            std::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<move_only> emplace 100", [](session_run &r) {
            htk::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<move_only> emplace 1000", [](session_run &r) {
            std::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 1000; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<move_only> emplace 1000", [](session_run &r) {
            htk::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 1000; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<move_only> emplace 10000", [](session_run &r) {
            std::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10000; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<move_only> emplace 10000", [](session_run &r) {
            htk::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 10000; ++i)
                    v.emplace_back(10);
            });
        });
    }

    {
        benchmark(s, "vector<move_only> emplace 100000", [](session_run &r) {
            std::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100000; ++i)
                    v.emplace_back(10);
            });
        });
    }
    {

        benchmark(s, "htk::vector<move_only> emplace 100000", [](session_run &r) {
            htk::vector<move_only> v;
            measure(r, [&v]() {
                for (int i = 0; i < 100000; ++i)
                    v.emplace_back(10);
            });
        });
    }
}


void measure_vector_int_insert_end(session &s)
{
    {
        benchmark(s, "vector<int> insert-end 1000", [](session_run &r) {
            std::vector<int> v = random_numeric_vector<int, std::vector<int>>(1000);
            std::vector<int> v2 = random_numeric_vector<int, std::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.end(), v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "htk::vector<int> insert-end 1000-1000", [](session_run &r) {
            auto v = random_numeric_vector<int, htk::vector<int>>(1000);
            auto v2 = random_numeric_vector<int, htk::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.end(), v2.begin(), v2.end());
            });
        });
    }
}

void measure_vector_int_insert_begin(session &s)
{
    {
        benchmark(s, "vector<int> insert-begin 1000-1000", [](session_run &r) {
            std::vector<int> v = random_numeric_vector<int, std::vector<int>>(1000);
            std::vector<int> v2 = random_numeric_vector<int, std::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.begin(), v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "htk::vector<int> insert-begin 1000-1000", [](session_run &r) {
            auto v = random_numeric_vector<int, htk::vector<int>>(1000);
            auto v2 = random_numeric_vector<int, htk::vector<int>>(1000);
            measure(r, [&v, &v2]() {
                v.insert(v.begin(), v2.begin(), v2.end());
            });
        });
    }
}

void measure_vector_int_insert_mid(session &s)
{
    {
        benchmark(s, "vector<int> insert-mid 1000-1000", [](session_run &r) {
            std::vector<int> v = random_numeric_vector<int, std::vector<int>>(1000);
            std::vector<int> v2 = random_numeric_vector<int, std::vector<int>>(1000);
            auto i = next_random(v.size() - 1);
            auto insert = v.begin() + i;
            measure(r, [&v, &v2, insert]() {
                v.insert(insert, v2.begin(), v2.end());
            });
        });
    }

    {
        benchmark(s, "htk::vector<int> insert-mid 1000-1000", [](session_run &r) {
            auto v = random_numeric_vector<int, htk::vector<int>>(1000);
            auto v2 = random_numeric_vector<int, htk::vector<int>>(1000);
            auto i = next_random(v.size() - 1);
            auto insert = v.begin() + i;
            measure(r, [&v, &v2, insert]() {
                v.insert(insert, v2.begin(), v2.end());
            });
        });
    }
}

void measure_linear_search(session &s)
{
    benchmark(s, "std::find vector<int>", { 10, 100, 1000, 10000, 100000, 1000000 }, [](session_run &r, int size) {
        auto v = random_numeric_vector<int, std::vector<int>>(size, 1, 100);
        auto value = next_random(1, 100);
        measure(r, [&v, &value]() {
            do_not_optimize(std::find(v.begin(), v.end(), value));
        });
    });
}

void measure_binary_search(session &s)
{
    benchmark(s, "std::binary_search vector<int>", { 10, 100, 1000, 10000, 100000, 1000000 }, [](session_run &r, int size) {
        auto v = random_numeric_vector<int, std::vector<int>>(size, 1, 100);
        std::sort(v.begin(), v.end());
        auto value = next_random(1, 100);
        measure(r, [&v, &value]() {
            do_not_optimize(std::binary_search(v.begin(), v.end(), value));
        });
    });
}

void measure_htk_binary_search(session &s)
{
    benchmark(s, "htk::binary_search vector<int>", { 10, 100, 1000, 10000, 100000, 1000000 }, [](session_run &r, int size) {
        auto v = random_numeric_vector<int, std::vector<int>>(size, 1, 100);
        std::sort(v.begin(), v.end());
        auto value = next_random(1, 100);
        measure(r, [&v, &value]() {
            do_not_optimize(htk::binary_search(v.begin(), v.end(), value));
        });
    });
}

void measure_htk_linear_search(session &s)
{
    benchmark(s, "htk::linear_search vector<int>", { 10, 100, 1000, 10000, 100000, 1000000 }, [](session_run &r, int size) {
        auto v = random_numeric_vector<int, std::vector<int>>(size, 1, 100);
        std::sort(v.begin(), v.end());
        auto value = next_random(1, 100);
        measure(r, [&v, &value]() {
            do_not_optimize(htk::linear_search(v.begin(), v.end(), value));
        });
    });
}

void measure_sort(session &s)
{
    benchmark(s, "std::sort vector<int>", { 10, 100, 1000, 10000, 100000, 1000000 }, [](session_run &r, int size) {
        auto v = random_numeric_vector<int, std::vector<int>>(size, 1, 100);

        measure(r, [&v]() {
            std::sort(v.begin(), v.end());
        });
    });
}

int main()
{
    session s;
    //measure_vector_int_emplace(s);
    //measure_vector_move_only_emplace(s);

    //measure_vector_int_insert_end(s);
    //measure_vector_int_insert_mid(s);
    //measure_vector_int_insert_begin(s);

    measure_linear_search(s);
    measure_binary_search(s);
    measure_htk_binary_search(s);
    measure_htk_linear_search(s);
    measure_sort(s);


    std::ofstream fs("vector_bench.csv");
    output(fs, s, tag_csv_format{});
}