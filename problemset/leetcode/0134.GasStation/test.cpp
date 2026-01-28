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

TEST(GasStation, BasicOperations) {
  Solution sol;

  {
    std::vector<int> gas{1, 2, 3, 4, 5};
    std::vector<int> cost{3, 4, 5, 1, 2};
    const int expected_result{3};
    const int actual_result{sol.canCompleteCircuit(gas, cost)};
    EXPECT_EQ(expected_result, actual_result);
  }

  {
    std::vector<int> gas{2, 3, 4};
    std::vector<int> cost{3, 4, 3};
    const int expected_result{-1};
    const int actual_result{sol.canCompleteCircuit(gas, cost)};
    EXPECT_EQ(expected_result, actual_result);
  }
}
