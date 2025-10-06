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

TEST(SymetricTree, BasicOperations) {
  EXPECT_TRUE(Solution::isSymmetric(TreeNode{1, {2, 2, 3, 4, 4, 3}}));
  EXPECT_FALSE(
      Solution::isSymmetric(TreeNode{1, {2, 2, nullptr, 3, nullptr, 3}}));
  EXPECT_FALSE(Solution::isSymmetric(TreeNode{2, {3, 3, 4, nullptr, 5, 4}}));
}
