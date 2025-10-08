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

TEST(MinimumPathSum, BasicOperations) {
  {
    std::vector<std::vector<int>> input{{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
    ASSERT_EQ(Solution::minPathSum(input), 7);
  }

  {
    std::vector<std::vector<int>> input{{1, 2, 3}, {4, 5, 6}};
    ASSERT_EQ(Solution::minPathSum(input), 12);
  }
}
