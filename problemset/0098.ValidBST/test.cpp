#include <lc_libs/tree_node_printer.hpp>

// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <vector>

using namespace lc_libs;
namespace log = lc_libs::log;

TEST(ValidBST, BasicOperations) {
  Solution sol;

  TreeNode root{2, {1, 3}};
  EXPECT_TRUE(sol.isValidBST(&root));

  TreeNode root2{5, {1, 4, nullptr, nullptr, 3, 6}};
  EXPECT_EQ(std::format("\n{}", root2.dfs()),
            "\n5,{1,null,null,4,{3,null,null,6,null,null}}");

  EXPECT_FALSE(sol.isValidBST(&root2));

  /*
  root =
      new TreeNode(5, {TreeNode(1), TreeNode(4, {TreeNode(3), TreeNode(6)})});
  EXPECT_FALSE(sol.isValidBST(root));
  delete root;

  root = new TreeNode(10,
                      {TreeNode(5), TreeNode(15, {TreeNode(6), TreeNode(20)})});
  EXPECT_FALSE(sol.isValidBST(root));
  delete root;

  root = new TreeNode(2147483647);
  EXPECT_TRUE(sol.isValidBST(root));
  delete root;
  */
}
