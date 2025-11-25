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

TEST(BinaryTreeMaximumPathSum, BasicOperations) {
  TreeNode{-10, {9, 20, nullptr, nullptr, 15, 7}};
  EXPECT_EQ(
      Solution::maxPathSumS(TreeNode{-10, {9, 20, nullptr, nullptr, 15, 7}}),
      42);

  EXPECT_EQ(Solution::maxPathSumS(TreeNode{1, {2, 3}}), 6);
  EXPECT_EQ(Solution::maxPathSumS(TreeNode{-3, {nullptr, nullptr}}), -3);
  EXPECT_EQ(Solution::maxPathSumS(TreeNode{2, {-1, nullptr, nullptr}}), 2);

  EXPECT_EQ(
      Solution::maxPathSumS(TreeNode{
          5, {4, 8, 11, nullptr, 13, 4, 7, 2, nullptr, nullptr, nullptr, 1}}),
      48);

  EXPECT_EQ(
      Solution::maxPathSumS(TreeNode{9,
                                     {6, -3, nullptr, nullptr, -6, 2, nullptr,
                                      nullptr, 2, nullptr, -6, -6, -6}}),
      16);
}
