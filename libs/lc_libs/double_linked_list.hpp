#pragma once

#include <format>
#include <initializer_list>
#include <memory>
#include <tuple>

namespace lc_libs {

template <typename T>
struct DListNodeBase;

template <typename T>
using DListView =
    typename std::tuple<const DListNodeBase<T>* const,
                        const DListNodeBase<T>* const, const char*>;

template <typename T>
struct DListNodeBase {
  using Value = T;
  using This = DListNodeBase<T>;
  using ThisPtr = std::add_pointer_t<This>;
  using ThisPtrConst = std::add_pointer_t<const This>;
  using UniquePtr = std::unique_ptr<This>;

  static const DListView<T> view(const DListNodeBase* first,
                                 const DListNodeBase* last, const char* sep) {
    return DListView<T>(first, last, sep);
  }

  const DListView<T> view(const char* sep = ",") const {
    return view(this, nullptr, sep);
  }

  Value val;
  DListNodeBase* next;
  DListNodeBase* prev;

  DListNodeBase() : val(0), next(nullptr), prev(nullptr) {}
  DListNodeBase(const DListNodeBase&) = delete;
  DListNodeBase(Value val, DListNodeBase* prev = nullptr,
                DListNodeBase* next = nullptr)
      : val{val}, next{next}, prev{prev} {}

  DListNodeBase(std::initializer_list<T> vals) : next(nullptr), prev(nullptr) {
    if (vals.size() == 0) {
      // Empty list
      val = 0;
      return;
    }

    auto it = vals.begin();
    val = *it;
    ++it;
    DListNodeBase* crt{this};
    for (; it != vals.end(); ++it) {
      auto newNode = new DListNodeBase(*it, crt);
      crt->next = newNode;
      crt = crt->next;
    }
  }

  /**
   * Unlink the next/prev nodes and return them. The current node will now point
   * to null as next & prev.
   * @return pair<prev, pnext>
   */
  std::pair<DListNodeBase*, DListNodeBase*> unlink() {
    auto next = this->next;
    auto prev = this->prev;

    this->next = nullptr;
    this->prev = nullptr;

    if (next && next->prev == this) {
      next->prev = prev;
    }

    if (prev && prev->next == this) {
      prev->next = next;
    }

    return {prev, next};
  }

  // Destructor to free the entire list with the exception of the head node
  ~DListNodeBase() { delete next; }
};

/**
 * Double lined list
 */
template <typename T>
class List {
 public:
  using Type = T;
  using Node = DListNodeBase<T>;

  List() = default;
  List(const List&) = delete;
  List(std::initializer_list<T> values) : _size(values.size()) {
    if (_size == 0) {
      return;
    }
    auto it = values.begin();
    _head = _tail = new Node(*it);
    ++it;

    for (; it != values.end(); ++it) {
      auto next = new Node(*it, _tail);
      _tail->next = next;
      _tail = next;
    }
  }

  ~List() { delete _head; }

  List& operator=(const List&) = delete;

  void push_back(T value) {
    ++_size;

    Node* node = new Node(value, nullptr, nullptr);
    if (_tail == nullptr) {
      _head = _tail = node;
      return;
    }
    _tail->next = node;
    _tail = node;

    if (_head->next == nullptr) {
      _head->next = _tail;
    }
  }

  void push_front(Node* node) {
    ++_size;
    if (empty()) {
      _head = _tail = node;
      return;
    }
    node->next = _head;
    _head = node;
  }

  void push_front_value(T value) { push_front(new Node(value)); }

  Node* back() { return _tail; }
  const Node* back() const { return _tail; }

  Node* front() { return _head; }
  const Node* front() const { return _head; }

  bool empty() const { return _size == 0; }
  size_t size() const { return _size; }

  /**
   * Remove the node from the list
   */
  void erase(Node* node) {
    if (node == _tail) {
      _tail = _tail->prev;
    }
    if (node == _head) {
      _head = _head->next;
    }
    node->unlink();
    --_size;
  }

  const DListView<T> view(const char* sep = ",") const {
    return DListNodeBase<T>::view(_head, _tail, sep);
  }

  /**
   * Compare two lists for equality.
   * Returns true if they are equal, false otherwise.
   */
  bool operator==(const List& other) const {
    const Node* l1{this->head_};
    const Node* l2{&other.head_};
    while (l1 && l2) {
      if (l1->val != l2->val) {
        return false;
      }
      l1 = l1->next;
      l2 = l2->next;
    }
    return l1 == nullptr && l2 == nullptr;
  }

  /**
   * Compare the list with an initializer list of values.
   * Returns true if they are equal, false otherwise.
   */
  bool operator==(const std::initializer_list<T> vals) const {
    auto it = vals.begin();
    const Node* crt{this->_head};
    for (; it != vals.end() && crt; ++it, crt = crt->next) {
      if (crt->val != *it) {
        return false;
      }
    }
    return it == vals.end() && crt == nullptr;
  }

  Node* at(size_t index) {
    auto crt = _head;
    while (crt != nullptr && index > 0) {
      crt = crt->next;
      --index;
    }
    return crt;
  }

  /**
   * Move a node to the head of the list
   */
  void relocate_front(Node* node) {
    if (node == _head) {
      return;  // no-op
    }

    if (node == _tail) {
      _tail = _tail->prev;
    }
    if (node->prev) {
      node->prev->next = node->next;
    }
    if (node->next) {
      node->next->prev = node->prev;
    }
    node->next = _head;
    node->prev = nullptr;
    _head = node;
  }

 private:
  Node* _head{};
  Node* _tail{};
  size_t _size{};
};

}  // namespace lc_libs

#if __has_include(<iostream>)
#include <iostream>
namespace lc_libs {

template <typename T>
std::ostream& operator<<(std::ostream& os, const DListNodeBase<T>& node) {
  const auto* crt = &node;
  while (crt) {
    os << crt->val;
    crt = crt->next;
    if (crt) os << ", ";
  }
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const DListView<T>& view) {
  bool add_separator = false;
  auto start = std::get<0>(view);
  auto end = std::get<1>(view);
  const char* sep = std::get<2>(view);

  for (auto it = start; it; it = it->next) {
    if (add_separator) os << sep;  // <-- removed () from sep
    os << it->val;
    add_separator = true;
    if (it == end) break;
  }
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& list) {
  return os << list.view();
}

}  // namespace lc_libs
#endif  // __has_include(<iostream>)

#if __has_include(<format>)
#include <format>
#include <tuple>

namespace std {

// Formatter for 3-tuple: (ListNodeBase<T>*, ListNodeBase<T>*, const char*)
template <typename T, typename CharT>
struct std::formatter<::lc_libs::DListView<T>, CharT> {
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ::lc_libs::DListView<T>& view, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    auto start = std::get<0>(view);
    auto end = std::get<1>(view);
    const char* sep = std::get<2>(view);

    auto out = ctx.out();
    bool first = true;
    for (auto it = start; it; it = it->next) {
      if (!first) {
        out = std::format_to(out, "{}", sep);
      }
      out = std::format_to(out, "{}", it->val);
      first = false;
      if (it == end) break;
    }
    return out;
  }
};

template <typename T, typename CharT>
struct std::formatter<::lc_libs::DListNodeBase<T>*, CharT>
    : std::formatter<std::basic_string<CharT>, CharT> {
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ::lc_libs::DListNodeBase<T>* const& node,
              FormatContext& ctx) const -> decltype(ctx.out()) {
    ::lc_libs::DListView<T> sep{node, nullptr, ", "};
    return std::formatter<decltype(sep), CharT>{}.format(sep, ctx);
  }
};

template <typename T, typename CharT>
struct std::formatter<::lc_libs::DListNodeBase<T>, CharT>
    : std::formatter<std::basic_string<CharT>, CharT> {
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ::lc_libs::DListNodeBase<T>& node, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    return std::formatter<::lc_libs::DListNodeBase<T>*, CharT>{}.format(&node,
                                                                        ctx);
  }
};

template <typename T, typename CharT>
struct std::formatter<::lc_libs::List<T>, CharT>
    : std::formatter<std::basic_string<CharT>, CharT> {
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ::lc_libs::List<T>& list, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    return std::formatter<::lc_libs::DListNodeBase<T>*, CharT>{}.format(
        list.front(), ctx);
  }
};

}  // namespace std
#endif  // __has_include(<format>)