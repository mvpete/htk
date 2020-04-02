
#include "gtest/gtest.h"
#include <htk/vector.h>

struct test_obj
{
    static unsigned int constructors;
    static unsigned int destructors;
    static unsigned int copies;
    static unsigned int copy_assigns;
    static unsigned int moves;
    static unsigned int move_assigns;

public:
    test_obj()
    {
        constructors++;
    }

    virtual ~test_obj()
    {
        destructors++;
    }

    test_obj(const test_obj& rhs)
    {
        copies++;
    }

    test_obj(test_obj&&)
    {
        moves++;
    }

    test_obj& operator=(const test_obj&)
    {
        copy_assigns++;
        return *this;
    }

    test_obj& operator=(test_obj&&)
    {
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

    std::shared_ptr<int> i;
};

struct moveable : public test_obj
{
    moveable()
        :test_obj()
    {
    }
    moveable(const moveable&) = delete;
    moveable(moveable&& m)
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
        :test_obj(std::move(c))
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

TEST(htk_stl_vector_tests, vector_initialize_pod) 
{
    htk::vector<int> v;
    EXPECT_EQ(0, v.size());
    EXPECT_EQ(0, v.capacity());
    EXPECT_TRUE(v.empty());
}

TEST(htk_stl_vector_tests, vector_initial_size)
{
    using vec = htk::vector<int>;
    vec v;
    v.emplace_back(1);
    EXPECT_EQ(vec::initial_capacity, v.capacity());
}

TEST(htk_stl_vector_tests, vector_emplace_pod)
{
    htk::vector<int> v;
    int i = v.emplace_back(1);
    EXPECT_EQ(1, i);
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

TEST(htk_stl_vector_tests, vector_expands_after_capacity)
{
    using vector = htk::vector<int>;
    vector v;
    for (int i = 0; i < vector::initial_capacity+1; ++i)
        v.emplace_back(i);

    EXPECT_EQ(vector::initial_capacity*2, v.capacity());
    EXPECT_EQ(vector::initial_capacity+1, v.size());
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

TEST(htk_stl_vector_tests, vector_moves_at_expand)
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

TEST(htk_stl_vector_tests, vector_copies_at_expand)
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