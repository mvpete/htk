

#include "gtest/gtest.h"
#include <htk/vector.h>

#include <array>


struct test_obj
{
    static unsigned int constructors;
    static unsigned int destructors;
    static unsigned int copies;
    static unsigned int copy_assigns;
    static unsigned int moves;
    static unsigned int move_assigns;

    uint8_t value_;

    test_obj()
        :value_(0)
    {
        constructors++;
    }

    test_obj(int value)
        :value_(value)
    {
    }

    virtual ~test_obj()
    {
        destructors++;
    }

    test_obj(const test_obj &rhs)
    {
        value_ = rhs.value_;
        copies++;
    }

    test_obj(test_obj &&rhs) noexcept
        :value_(0)
    {
        std::swap(value_, rhs.value_);
        moves++;
    }

    test_obj& operator=(const test_obj &rhs)
    {
        value_ = rhs.value_;
        copy_assigns++;
        return *this;
    }

    test_obj& operator=(test_obj &&rhs)
    {
        std::swap(value_, rhs.value_);
        move_assigns++;
        return *this;
    }

    static void reset()
    {
        constructors = 0;
        destructors = 0;
        copies = 0;
        copy_assigns = 0;
        moves = 0;
        move_assigns = 0;

    }
};

inline bool operator==(const test_obj& lhs, const test_obj& rhs) { return lhs.value_ == rhs.value_;  }

struct moveable : public test_obj
{
    moveable()
        :test_obj()
    {
    }
    moveable(const moveable&) = delete;
    moveable(moveable&& m) noexcept
        :test_obj(std::move(m))
    {
    }
};

struct copyable : public test_obj
{
    copyable()
        :test_obj()
    {
    }
    copyable(const copyable& c)
        :test_obj(c)
    {
    }
    copyable(copyable&&) = delete;
};

unsigned int test_obj::constructors = 0;
unsigned int test_obj::destructors = 0;
unsigned int test_obj::copies = 0;
unsigned int test_obj::copy_assigns= 0;
unsigned int test_obj::moves = 0;
unsigned int test_obj::move_assigns = 0;


template <typename T, typename Vec, size_t size>
void expect_eq(std::array<T, size> array, const Vec& vec)
{
    EXPECT_EQ(array.size(), vec.size());
    for (size_t i = 0; i < array.size(); ++i)
        EXPECT_EQ(array.at(i), vec.at(i));
}

template <typename T, typename Vec>
void expect_eq(const std::initializer_list<T> &array, const Vec& vec)
{
    EXPECT_EQ(array.size(), vec.size());
    auto begin = array.begin();
    for (size_t i = 0; i < array.size(); ++i, ++begin)
        EXPECT_EQ(*begin, vec.at(i));
}

TEST(htk_stl_vector_tests, vector_initialize_pod) 
{
    htk::vector<int> v;
    EXPECT_EQ(0, v.size());
    EXPECT_EQ(0, v.capacity());
    EXPECT_TRUE(v.empty());
}

TEST(htk_stl_vector_tests, vector_destructor)
{
    {
        htk::vector<test_obj> v;
        v.emplace_back();
    }
    EXPECT_EQ(1, test_obj::constructors);
    EXPECT_EQ(1, test_obj::destructors);
}

TEST(htk_stl_vector_tests, vector_initial_size)
{
    using vec = htk::vector<int>;
    vec v;
    v.emplace_back(1);
    EXPECT_EQ(vec::initial_capacity, v.capacity());
}

TEST(htk_stl_vector_tests, vector_emplace_back_pod)
{
    htk::vector<int> v;
    int i = v.emplace_back(1);
    EXPECT_EQ(1, i);
    EXPECT_EQ(1, v.size());
}

TEST(htk_stl_vector_tests, vector_push_back_pod)
{
    htk::vector<int> v;
    v.push_back(1);

    EXPECT_EQ(1, v.at(0));
    EXPECT_EQ(1, v.size());
}

TEST(htk_stl_vector_tests, vector_emplace_back_to_capacity)
{
    using vector = htk::vector<int>;
    vector v;
    for (int i = 0; i < vector::initial_capacity; ++i)
        v.emplace_back(i);

    EXPECT_EQ(vector::initial_capacity, v.capacity());
    EXPECT_EQ(vector::initial_capacity, v.size());
}

TEST(htk_stl_vector_tests, vector_emplace_back_expands_after_capacity)
{
    using vector = htk::vector<int>;
    vector v;
    for (int i = 0; i < vector::initial_capacity+1; ++i)
        v.emplace_back(i);

    EXPECT_EQ(vector::initial_capacity*2, v.capacity());
    EXPECT_EQ(vector::initial_capacity+1, v.size());
}

TEST(htk_stl_vector_tests, vector_emplace_back_moves_at_expand)
{
    test_obj::reset();

    using vector = htk::vector<moveable>;
    vector v;
    for (int i = 0; i < vector::initial_capacity + 1; ++i)
    {
        v.emplace_back();
    }

    EXPECT_EQ(vector::initial_capacity + 1, test_obj::constructors);
    EXPECT_EQ(vector::initial_capacity, test_obj::moves);
    EXPECT_EQ(0, test_obj::copies);
}

TEST(htk_stl_vector_tests, vector_emplace_back_copies_at_expand)
{
    test_obj::reset();

    using vector = htk::vector<copyable>;
    vector v;
    for (int i = 0; i < vector::initial_capacity + 1; ++i)
    {
        v.emplace_back();
    }

    EXPECT_EQ(vector::initial_capacity + 1, test_obj::constructors);
    EXPECT_EQ(vector::initial_capacity, test_obj::copies);
    EXPECT_EQ(0, test_obj::moves);
}

TEST(htk_stl_vector_tests, vector_moves_opt_at_expand)
{
    test_obj::reset();

    using vector = htk::vector<test_obj>;
    vector v;
    for (int i = 0; i < vector::initial_capacity + 1; ++i)
    {
        v.emplace_back();
    }

    EXPECT_EQ(vector::initial_capacity + 1, test_obj::constructors);
    EXPECT_EQ(vector::initial_capacity, test_obj::moves);
    EXPECT_EQ(0, test_obj::copies);
}

// insert value
TEST(htk_stl_vector_tests, vector_pod_insert_one_begin_has_capacity)
{
    htk::vector<int> v;

    v.emplace_back(2);
    v.emplace_back(3);
    v.emplace_back(4);

    const auto begin = v.begin();
    v.insert(begin, 1, 1);

    EXPECT_EQ(4, v.size());
    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());
    auto test = v.begin();
    for (int i = 1; i < 5; ++i, ++test)
        EXPECT_EQ(i, *test);

}

TEST(htk_stl_vector_tests, vector_pod_insert_multi_begin_has_capacity)
{
    htk::vector<int> v;

    v.emplace_back(2);
    v.emplace_back(3);
    v.emplace_back(4);

    const auto begin = v.begin();
    v.insert(begin, 3, 1);

    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());
    expect_eq({ 1,1,1,2,3,4 }, v);

}

TEST(htk_stl_vector_tests, vector_pod_insert_one_middle_has_capacity)
{
    htk::vector<int> v;

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(4);

    const auto middle = v.begin()+2;
    v.insert(middle, 1, 3);

    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());
}

TEST(htk_stl_vector_tests, vector_pod_insert_multi_middle_has_capacity)
{
    htk::vector<uint8_t> v;

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(4);
    v.emplace_back(5);
    v.emplace_back(6);

    const auto begin = v.begin()+2;
    v.insert(begin, 3, 3);

    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());
    expect_eq({ uint8_t{1},uint8_t{2},uint8_t{3},uint8_t{3},uint8_t{3},uint8_t{4},uint8_t{5},uint8_t{6} }, v);

}

TEST(htk_stl_vector_tests, vector_class_insert_multi_middle_has_capacity)
{
    htk::vector<test_obj> v;
    //v.reserve(10);

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(4);
    v.emplace_back(5);
    v.emplace_back(6);

    const auto begin = v.begin() + 2;
    v.insert(begin, 3, 3);

    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());
    expect_eq({ test_obj(1),test_obj(2),test_obj(3),test_obj(3),test_obj(3),test_obj(4),test_obj(5),test_obj(6) }, v);

}

TEST(htk_stl_vector_tests, vector_pod_insert_one_end_has_capacity)
{
    htk::vector<int> v;

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    const auto end = v.end();
    v.insert(end, 1, 4);

    EXPECT_EQ(4, v.size());
    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());

}

TEST(htk_stl_vector_tests, vector_pod_insert_multi_end_has_capacity)
{
    htk::vector<int> v;

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    v.insert(v.end(), 3, 4);

    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());
    expect_eq({ 1,2,3,4,4,4 }, v);

}


// insert range
TEST(htk_stl_vector_tests, vector_pod_insert_range_end_has_capacity)
{
    htk::vector<int> v;

    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);

    htk::vector<int> v2;
    v2.emplace_back(4);
    v2.emplace_back(5);
    v2.emplace_back(6);

    int arr[3];

    //v.insert(v.end(), v2.begin(), v2.end());

    EXPECT_EQ(htk::vector<int>::initial_capacity, v.capacity());
    expect_eq({ 1,2,3,4,5,6 }, v);

}

TEST(htk_stl_vector_tests, vector_pod_insert_range_begin_has_capacity)
{
    EXPECT_TRUE(false);
}

TEST(htk_stl_vector_tests, vector_pod_insert_range_middle_has_capacity)
{
    EXPECT_TRUE(false);
}


// clear
TEST(htk_stl_vector_tests, vector_clear_pod)
{
    // test that we destroy all the itesm
    htk::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.clear();

    EXPECT_EQ(0, v.size());
}

TEST(htk_stl_vector_tests, vector_clear_after_expand_pod)
{
    htk::vector<int> v;
    for (int i = 0; i < 11; ++i)
        v.push_back(i);
    v.clear();

    EXPECT_EQ(0, v.size());
}

TEST(htk_stl_vector_tests, vector_clear_npod)
{
    test_obj::reset();
    htk::vector<test_obj> v;
    for (int i = 0; i < 5; ++i)
        v.emplace_back();

    v.clear();
    EXPECT_EQ(0, v.size());
    EXPECT_EQ(5, test_obj::destructors);
}

TEST(htk_stl_vector_tests, vector_clear_after_expand_npod)
{
    test_obj::reset();
    htk::vector<test_obj> v;
    for (int i = 0; i < 15; ++i)
        v.emplace_back();

    v.clear();
    EXPECT_EQ(0, v.size());
    // 10 for the expansion + 15 for cleanup
    EXPECT_EQ(25, test_obj::destructors);
}


// algorithms
TEST(htk_stl_vector_tests, vector_for)
{
    htk::vector<int> v;
    v.emplace_back(0);
    v.emplace_back(1);
    v.emplace_back(2);

    int e = 0;
    for (const int i : v)
    {
        EXPECT_EQ(e++, i);
    }
}

TEST(htk_stl_vector_tests, vector_std_foreach)
{
    htk::vector<int> v;
    v.emplace_back(0);
    v.emplace_back(1);
    v.emplace_back(2);

    int e = 0;
    std::for_each(v.begin(), v.end(), [&e](int v)
    {
        EXPECT_EQ(e++, v);
    });
}

TEST(htk_stl_vector_tests, vector_sort)
{
    htk::vector<int> v;
    v.emplace_back(3);
    v.emplace_back(7);
    v.emplace_back(0);
    std::sort(v.begin(), v.end());
}

