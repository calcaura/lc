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

TEST(BinaryTreeZigzagLevelOrderTraversal, BasicOperations) {
  using R = std::vector<std::vector<int>>;

  EXPECT_EQ(
      Solution::zigzagLevelOrder(TreeNode{3, {9, 20, nullptr, nullptr, 15, 7}}),
      (R{{3}, {20, 9}, {15, 7}}));

  EXPECT_EQ(
      Solution::zigzagLevelOrder(TreeNode{1, {2, 3, 4, nullptr, nullptr, 5}}),
      (R{{1}, {3, 2}, {4, 5}}));

  EXPECT_EQ(Solution::zigzagLevelOrder(TreeNode{1}), (R{{1}}));
  EXPECT_EQ(Solution::zigzagLevelOrder(nullptr), (R{}));
}
