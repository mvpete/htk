#include "gtest/gtest.h"
#include <htk/algorithm.h>

TEST(htk_algorithm_tests, test_binary_search_simple_true)
{
    std::vector<int> v { 1, 2, 3, 4 };
    EXPECT_TRUE(htk::binary_search(v.begin(), v.end(), 1));
    EXPECT_TRUE(htk::binary_search(v.begin(), v.end(), 2));
    EXPECT_TRUE(htk::binary_search(v.begin(), v.end(), 3));
    EXPECT_TRUE(htk::binary_search(v.begin(), v.end(), 4));

    std::vector<int> v2{ 1 };
    EXPECT_TRUE(htk::binary_search(v2.begin(), v2.end(), 1));
}

TEST(htk_algorithm_tests, test_binary_search_simple_false)
{
    std::vector<int> v{ 1, 2, 3, 4 };
    EXPECT_FALSE(htk::binary_search(v.begin(), v.end(), -1));
    EXPECT_FALSE(htk::binary_search(v.begin(), v.end(), 0));
    EXPECT_FALSE(htk::binary_search(v.begin(), v.end(), 5));
}

