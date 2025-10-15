#pragma once

#include <lc_libs/tree_node_printer.hpp>
#include <memory>
#include <queue>

using TreeNode = lc_libs::TreeNodeBase<int>;
class Solution {
 public:
  static bool isSameTree(const TreeNode& p, const TreeNode& q) {
    return isSameTree(&p, &q);
  }

  static bool isSameTree(const TreeNode* p, const TreeNode* q) {
    std::deque<const TreeNode*> a, b;
    a.push_back(p);
    b.push_back(q);

    while (!a.empty()) {
      auto ln = a.front();
      a.pop_front();
      if (b.empty()) {
        return false;
      }
      auto lr = b.front();
      b.pop_front();

      if (ln == nullptr) {
        if (lr != nullptr) {
          return false;
        }
      } else {
        if (lr == nullptr) {
          return false;
        }
        if (ln->val != lr->val) {
          return false;
        }
        a.push_back(ln->left);
        a.push_back(ln->right);

        b.push_back(lr->left);
        b.push_back(lr->right);
      }
    }
    return b.empty();
  }
};