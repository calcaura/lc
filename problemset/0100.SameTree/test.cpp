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
  EXPECT_TRUE(Solution::isSameTree(TreeNode{1, {2, 3}}, TreeNode{1, {2, 3}}));
  EXPECT_FALSE(
      Solution::isSameTree(TreeNode{1, {2}}, TreeNode{1, {nullptr, 2}}));

  EXPECT_FALSE(Solution::isSameTree(TreeNode{1, {2, 1}}, TreeNode{1, {1, 2}}));
}
