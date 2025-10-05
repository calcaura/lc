#pragma once
#include <any>
#include <format>
#include <initializer_list>
#include <iterator>
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
  using Node = TreeNodeBase<T>;
  using Iterator = typename Node::DfsIterator;
  Node* node;
  DfsView(Node* n) : node(n) {}

  Iterator begin() const { return Iterator(node); }
  Iterator end() const { return Iterator(); }
};

template <typename T>
struct BfsView {
  using Node = TreeNodeBase<T>;
  using Iterator = typename Node::BfsIterator;
  Node* node;
  BfsView(Node* n) : node(n) {}

  Iterator begin() { return Iterator(node); }
  Iterator end() { return Iterator(); }
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

  DfsView<T> dfs_view() { return {this}; }
  BfsView<T> bfs_view() { return {this}; }

  bool no_children() const { return left == nullptr && right == nullptr; }

  class DfsIterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;

    DfsIterator() = default;
    DfsIterator(TreeNodeBase* root) { push_children(root); }

    DfsIterator operator++(int) {
      auto result = *this;
      ++*this;
      return result;
    }

    DfsIterator& operator++() {
      if (_queue.empty()) {
        // no-op
        return *this;
      }
      auto n = _queue.front();
      _queue.pop_front();
      if (!n->right) {
        return *this;
      }
      if (_queue.empty()) {
        push_children(n->right);
      } else {
        auto head = _queue.front();
        _queue.pop_front();
        push_children(n->right);
        _queue.push_front(head);
      }
      return *this;
    }

    operator T() const { return _queue.front()->val; }
    operator T&() const { return _queue.front()->val; }
    operator T&() { return _queue.front()->val; }

    T operator*() const { return _queue.front()->val; }

    bool operator==(const DfsIterator& o) const {
      if (_queue.empty()) {
        return o._queue.empty();
      } else if (o._queue.empty()) {
        return false;
      }
      return _queue.front() == o._queue.front();
    }

   private:
    std::deque<TreeNodeBase*> _queue;
    void push_children(TreeNodeBase* node) {
      while (node) {
        _queue.push_front(node);
        node = node->left;
      }
    }
  };
  static_assert(std::forward_iterator<DfsIterator>);

  class BfsIterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;

    BfsIterator() = default;
    BfsIterator(TreeNodeBase* root) { _queue.push_front(root); }

    BfsIterator operator++(int) {
      auto result = *this;
      ++*this;
      return result;
    }

    BfsIterator& operator++() {
      if (_queue.empty()) {
        // no-op
        return *this;
      }
      auto n = _queue.front();
      _queue.pop_front();
      if (n->left) {
        _queue.push_back(n->left);
      }
      if (n->right) {
        _queue.push_back(n->right);
      }
      return *this;
    }

    operator T() const { return _queue.front()->val; }
    operator T&() const { return _queue.front()->val; }
    operator T&() { return _queue.front()->val; }

    T operator*() const { return _queue.front()->val; }

    bool operator==(const BfsIterator& o) const {
      if (_queue.empty()) {
        return o._queue.empty();
      } else if (o._queue.empty()) {
        return false;
      }
      return _queue.front()->val == o._queue.front()->val;
    }

   private:
    std::deque<TreeNodeBase*> _queue;
  };
  static_assert(std::forward_iterator<BfsIterator>);
};

}  // namespace lc_libs
