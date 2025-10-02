#pragma once
#include <any>
#include <format>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <queue>
#include <span>
#include <tuple>
#include <vector>

namespace lc_libs {

template <typename T>
struct TreeNodeBase;

template <typename T>
struct DfsView {
  const TreeNodeBase<T>* const node;
  DfsView(const TreeNodeBase<T>* const n) : node(n) {}
};

template <typename T>
struct BfsView {
  const TreeNodeBase<T>* node;
  BfsView(const TreeNodeBase<T>* n) : node(n) {}
};

template <typename T>
struct TreeNodeBase {
  using This = TreeNodeBase<T>;
  using ThisPtr = std::add_pointer_t<This>;
  using UniquePtr = std::unique_ptr<This>;
  using SharedPtr = std::shared_ptr<This>;

  T val;
  TreeNodeBase* left;
  TreeNodeBase* right;
  TreeNodeBase() : val(0), left(nullptr), right(nullptr) {}
  TreeNodeBase(T x) : val(x), left(nullptr), right(nullptr) {}
  TreeNodeBase(T x, TreeNodeBase* left, TreeNodeBase* right)
      : val(x), left(left), right(right) {}

  TreeNodeBase(T x, std::span<const std::any> nodes) : val(x), left{}, right{} {
    // Build tree from LeetCode-style level-order array after root: [l, r, ...]
    std::queue<TreeNodeBase<T>*> q;
    q.push(this);
    std::size_t idx = 0;
    while (!q.empty() && idx < nodes.size()) {
      TreeNodeBase<T>* current = q.front();
      q.pop();

      // Left child
      if (idx < nodes.size()) {
        if (nodes[idx].type() == typeid(T)) {
          current->left = new TreeNodeBase<T>(std::any_cast<T>(nodes[idx]));
          q.push(current->left);
        } else if (nodes[idx].type() == typeid(std::nullptr_t)) {
          current->left = nullptr;
        } else {
          throw std::runtime_error("Unsupported type for left node");
        }
        ++idx;
      }

      // Right child
      if (idx < nodes.size()) {
        if (nodes[idx].type() == typeid(T)) {
          current->right = new TreeNodeBase<T>(std::any_cast<T>(nodes[idx]));
          q.push(current->right);
        } else if (nodes[idx].type() == typeid(std::nullptr_t)) {
          current->right = nullptr;
        } else {
          throw std::runtime_error("Unsupported type for right node");
        }
        ++idx;
      }
    }
  }

  TreeNodeBase(T x, std::initializer_list<const std::any> nodes)
      : TreeNodeBase(x, std::span<const std::any>(nodes)) {}

  /**
   * Compare two trees for equality.
   * Returns true if they are equal, false otherwise.
   */
  bool operator==(const TreeNodeBase& other) const {
    const TreeNodeBase* l1{this};
    const TreeNodeBase* l2{&other};
    if (!l1 && !l2) return true;
    if (!l1 || !l2) return false;
    if (l1->val != l2->val) return false;
    return (l1->left == nullptr ? l2->left == nullptr
                                : *l1->left == *l2->left) &&
           (l1->right == nullptr ? l2->right == nullptr
                                 : *l1->right == *l2->right);
  }

  ~TreeNodeBase() {
    if (left) {
      delete left;
      left = nullptr;
    }
    if (right) {
      delete right;
      right = nullptr;
    }
  }

  DfsView<T> dfs() const { return DfsView<T>(this); }
  BfsView<T> bfs() const { return BfsView<T>(this); }

  static void print_dfs(std::ostream& os, const TreeNodeBase<T>* const root) {
    if (!root) {
      os << "null";
      return;
    }
    os << root->val;
    if (!root->left && !root->right) {
      os << ",null,null";
      return;
    }
    if (root->left || root->right) {
      os << ",{";
      print_dfs(os, root->left);
      os << ",";
      print_dfs(os, root->right);
      os << "}";
    }
  }

  template <typename FormatContext>
  static auto print_dfs(FormatContext& ctx, const TreeNodeBase<T>* const root)
      -> typename FormatContext::iterator {
    if (!root) {
      return std::format_to(ctx.out(), "null");
    }
    auto it = std::format_to(ctx.out(), "{}", root->val);
    if (!root->left && !root->right) {
      it = std::format_to(it, ",null,null");
      return it;
    }
    if (root->left || root->right) {
      // When using std::format_to, '{{' -> literal '{' and '}}' -> literal '}'.
      it = std::format_to(it, ",{{");
      it = print_dfs(ctx, root->left);
      it = std::format_to(it, ",");
      it = print_dfs(ctx, root->right);
      it = std::format_to(it, "}}");
    }
    return it;
  }

  // Improved BFS printer: stop after last meaningful level
  static void print_bfs(std::ostream& os, const TreeNodeBase<T>* const root) {
    if (!root) {
      os << "null";
      return;
    }
    std::vector<const TreeNodeBase<T>*> current{root};
    while (!current.empty()) {
      // print current level
      bool any_non_null = false;
      for (const auto* n : current) {
        if (n) {
          os << n->val << " ";
          any_non_null = true;
        } else {
          os << "null ";
        }
      }
      os << "\n";
      if (!any_non_null) break;

      // build next level
      std::vector<const TreeNodeBase<T>*> next;
      next.reserve(current.size() * 2);
      for (const auto* n : current) {
        if (n) {
          next.push_back(n->left);
          next.push_back(n->right);
        } else {
          // keep placeholders so positions remain aligned
          next.push_back(nullptr);
          next.push_back(nullptr);
        }
      }

      // Trim trailing nulls in `next` to avoid infinite expansion:
      while (!next.empty() && next.back() == nullptr) next.pop_back();
      current.swap(next);
    }
  }
};

}  // namespace lc_libs

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const typename ::lc_libs::DfsView<T>& view) {
  ::lc_libs::TreeNodeBase<T>::print_dfs(os, view.node);
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const typename ::lc_libs::BfsView<T>& view) {
  ::lc_libs::TreeNodeBase<T>::print_bfs(os, view.node);
  return os;
}
