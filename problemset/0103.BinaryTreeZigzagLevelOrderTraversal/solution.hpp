#pragma once
#include <algorithm>
#include <deque>
#include <lc_libs/tree_node.hpp>
#include <vector>

using TreeNode = lc_libs::TreeNodeBase<int>;

class Solution {
 public:
  static std::vector<std::vector<int>> zigzagLevelOrder(const TreeNode& root) {
    return zigzagLevelOrder(&root);
  }

  static std::vector<std::vector<int>> zigzagLevelOrder(const TreeNode* root) {
    if (!root) {
      return {};
    }
    std::deque<std::pair<const TreeNode*, size_t>> queue;
    std::vector<std::vector<int>> result;

    queue.push_back({root, 0});
    result.push_back({});

    auto add_node = [&queue](TreeNode* node, size_t next_level) {
      if (node != nullptr) {
        queue.push_back({node, next_level});
      }
    };

    while (!queue.empty()) {
      auto [node, level] = queue.front();
      queue.pop_front();

      auto next_level = level + 1;
      result[level].push_back(node->val);

      if (next_level >= result.size()) {
        result.push_back({});
      }

      add_node(node->left, next_level);
      add_node(node->right, next_level);
    }
    if (result.back().empty()) {
      result.pop_back();
    }
    for (size_t it = 1; it < result.size(); it += 2) {
      std::reverse(result[it].begin(), result[it].end());
    }
    return result;
  }
};