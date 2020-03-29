
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
    ~test_obj()
    {
        destructors++;
    }

    test_obj(const test_obj&)
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
    v.emplace_back(1);
    EXPECT_EQ(1, v.size());
}

TEST(htk_stl_vector_tests, vector_destructor)
{
    htk::vector<test_obj> v;

}