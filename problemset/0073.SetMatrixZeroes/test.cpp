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

TEST(SetMatrixZeroes, BasicOperations) {
  std::vector<std::vector<int>> matrix{
      {1, 1, 1},
      {1, 0, 1},
      {1, 1, 1},
  };
  Solution::setZeroes(matrix);
  EXPECT_EQ(matrix, (std::vector<std::vector<int>>{
                        {1, 0, 1},
                        {0, 0, 0},
                        {1, 0, 1},
                    }));

  matrix = {
      {0, 1, 2, 0},
      {3, 4, 5, 2},
      {1, 3, 1, 5},
  };
  Solution::setZeroes(matrix);
  EXPECT_EQ(matrix, (std::vector<std::vector<int>>{
                        {0, 0, 0, 0},
                        {0, 4, 5, 0},
                        {0, 3, 1, 0},
                    }));
}
