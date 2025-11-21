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

TEST(MaxPointsonaLine, BasicOperations) {
  std::vector<std::vector<int>> points1{{1, 1}, {2, 2}, {3, 3}};
  EXPECT_EQ(Solution::maxPoints(points1), 3);

  std::vector<std::vector<int>> points2{{1, 1}, {3, 2}, {5, 3},
                                        {4, 1}, {2, 3}, {1, 4}};
  EXPECT_EQ(Solution::maxPoints(points2), 4);

  std::vector<std::vector<int>> points3{
      {0, 0}, {94911151, 94911150}, {94911152, 94911151}};
  EXPECT_EQ(Solution::maxPoints(points3), 2);
}
