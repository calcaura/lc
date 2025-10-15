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

TEST(RotateImage, BasicOperations) {
  using Matrix = Solution::Matrix;

  Matrix m1 = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
  };

  Matrix m1_rotated = {
      {7, 4, 1},
      {8, 5, 2},
      {9, 6, 3},
  };

  Solution::rotate(m1);
  EXPECT_EQ(m1, m1_rotated);

  Matrix m2 = {
      {5, 1, 9, 11},
      {2, 4, 8, 10},
      {13, 3, 6, 7},
      {15, 14, 12, 16},
  };

  Matrix m2_rotated = {
      {15, 13, 2, 5},
      {14, 3, 4, 1},
      {12, 6, 8, 9},
      {16, 7, 10, 11},
  };

  Solution::rotate(m2);
  EXPECT_EQ(m2, m2_rotated);
}
