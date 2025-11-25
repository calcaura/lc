#pragma once

#include <algorithm>
#include <lc_libs/tree_node.hpp>
#include <limits>
#include <utility>

using sum_t = std::pair<int, int>;
using TreeNode = lc_libs::TreeNodeBase<int>;

class Solution {
 public:
  static inline constexpr int INVALID{std::numeric_limits<int>::min()};
  int _max;

  static int maxPathSumS(const TreeNode& root) {
    Solution sol;
    return sol.maxPathSum(&root);
  }

  int maxPathSum(const TreeNode* root) {
    _max = INVALID;
    auto res = msum(root);
    return std::max(_max, std::max(res.first, res.second));
  }

  sum_t msum(const TreeNode* root) {
    if (!root) {
      return {0, 0};
    }

    sum_t result{};

    auto left = msum(root->left);
    auto right = msum(root->right);

    if (left.first > 0) {
      result.second += left.first;
    }
    if (right.first > 0) {
      result.second += right.first;
    }
    result.second += root->val;

    if (result.second > _max) {
      _max = result.second;
    }

    result.first =
        root->val + std::max(std::max(left.first, 0), std::max(right.first, 0));
    if (result.first > _max) {
      _max = result.first;
    }

    return result;
  }
};