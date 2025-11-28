// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <vector>

namespace log = lc_libs::log;

TEST(Subsets, BasicOperations) {
  {
    std::vector<int> nums{1, 2, 3};
    const auto result = Solution::subsets(nums);
    std::vector<std::vector<int>> expected{
        {}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3},
    };
    EXPECT_EQ(result.size(), expected.size());
    for (const auto& subset : expected) {
      EXPECT_NE(std::find(result.begin(), result.end(), subset), result.end());
    }
  }

  {
    std::vector<int> nums{0};
    const auto result = Solution::subsets(nums);
    std::vector<std::vector<int>> expected{
        {},
        {0},
    };
    EXPECT_EQ(result.size(), expected.size());
    for (const auto& subset : expected) {
      EXPECT_NE(std::find(result.begin(), result.end(), subset), result.end());
    }
  }
}
