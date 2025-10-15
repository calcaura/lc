#pragma once

#include <lc_libs/tree_node_printer.hpp>
#include <memory>
#include <queue>

using TreeNode = lc_libs::TreeNodeBase<int>;
class Solution {
 public:
  static bool isSymmetric(const TreeNode& root) {
    return Solution::isSymmetric(&root);
  }

  static bool isSymmetric(const TreeNode* root) {
    std::deque<TreeNode*> left, right;
    if (root->left) {
      if (!root->right) {
        return false;
      }
    } else if (root->right) {
      return false;
    } else {
      return true;
    }
    left.push_back(root->left);
    right.push_back(root->right);
    while (!left.empty()) {
      auto l = left.front();
      auto r = right.front();
      if (l->val != r->val) {
        return false;
      }
      left.pop_front();
      right.pop_front();
      if (l->left) {
        if (r->right) {
          left.push_back(l->left);
          right.push_back(r->right);
        } else {
          return false;
        }
      } else {
        if (r->right) {
          return false;
        }
      }
      if (l->right) {
        if (r->left) {
          left.push_back(l->right);
          right.push_back(r->left);
        } else {
          return false;
        }
      } else {
        if (r->left) {
          return false;
        }
      }
    }
    return right.empty();
  }
};