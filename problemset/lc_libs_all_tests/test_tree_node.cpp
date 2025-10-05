
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <lc_libs/tree_node.hpp>
#include <vector>

namespace log = lc_libs::log;
using TreeNode = lc_libs::TreeNodeBase<int>;

TEST(TreeNodeTest, BasicOperations) {
  TreeNode root{2,
                {1, 4, nullptr, nullptr, 3, 6, nullptr, nullptr, nullptr, 7}};

  ASSERT_EQ(root.val, 2);

  ASSERT_EQ(root.left->val, 1);
  ASSERT_EQ(root.right->val, 4);

  ASSERT_TRUE(root.left->no_children());

  ASSERT_EQ(root.right->left->val, 3);
  ASSERT_EQ(root.right->right->val, 6);

  ASSERT_TRUE(root.right->left->no_children());
  ASSERT_EQ(root.right->left->left, nullptr);

  ASSERT_EQ(root.height(), 4);

  {
    auto view = root.dfs_view();
    std::vector<int> ser(view.begin(), view.end());
    ASSERT_EQ((std::vector<int>{1, 2, 3, 4, 6, 7}), ser);
  }

  {
    auto view = root.bfs_view();
    std::vector<int> ser(view.begin(), view.end());
    ASSERT_EQ((std::vector<int>{2, 1, 4, 3, 6, 7}), ser);
  }
}
