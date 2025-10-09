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

TEST(RemoveDuplicatesfromSortedArrayII, BasicOperations) {
  {
    std::vector<int> input{1, 1, 1, 2, 2, 3};
    const std::vector<int> expected{1, 1, 2, 2, 3};

    auto res = Solution::removeDuplicates(input);
    ASSERT_EQ(res, expected.size());
    input.resize(expected.size());
    ASSERT_EQ(input, expected);
  }

  {
    std::vector<int> input{0, 0, 1, 1, 1, 1, 2, 3, 3};
    const std::vector<int> expected{0, 0, 1, 1, 2, 3, 3};

    auto res = Solution::removeDuplicates(input);
    ASSERT_EQ(res, expected.size());
    input.resize(expected.size());
    ASSERT_EQ(input, expected);
  }
}
