#pragma once

#include <lc_libs/tree_node_printer.hpp>
#include <limits>
#include <memory>

using TreeNode = lc_libs::TreeNodeBase<int>;

class Solution {
 public:
  bool isValidBST(TreeNode* root) {
    return isValidBST(root->left, std::numeric_limits<int64_t>::min(),
                      root->val) &&
           isValidBST(root->right, root->val,
                      std::numeric_limits<int64_t>::max());
  }

  bool isValidBST(TreeNode* root, int64_t min, int64_t max) {
    if (!root) return true;

    auto val = root->val;
    if (val <= min || val >= max) {
      return false;
    }
    return isValidBST(root->left, min, val) &&
           isValidBST(root->right, val, max);
  }
};