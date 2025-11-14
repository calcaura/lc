#pragma once
#include <format>
#include <initializer_list>
#include <memory>
#include <tuple>

namespace lc_libs {

template <typename T>
struct ListNodeBase;

template <typename T>
using ListView = typename std::tuple<const ListNodeBase<T>* const,
                                     const ListNodeBase<T>* const, const char*>;

template <typename T>
struct ListNodeBase {
  using This = ListNodeBase<T>;
  using ThisPtr = std::add_pointer_t<This>;
  using UniquePtr = std::unique_ptr<This>;

  const ListView<T> view(const char* sep) const {
    return ListView<T>(this, nullptr, sep);
  }

  static const ListView<T> view(ListNodeBase* first, ListNodeBase* last,
                                const char* sep) {
    return ListView<T>(first, last, sep);
  }

  T val;
  ListNodeBase* next;
  ListNodeBase() : val(0), next(nullptr) {}
  ListNodeBase(T x) : val(x), next(nullptr) {}
  ListNodeBase(T x, ListNodeBase* next) : val(x), next(next) {}
  ListNodeBase(std::initializer_list<T> vals) : next(nullptr) {
    if (vals.size() == 0) {
      // Empty list
      val = 0;
      next = nullptr;
      return;
    }
    auto it = vals.begin();
    val = *it;
    ++it;
    ListNodeBase* crt{this};
    for (; it != vals.end(); ++it) {
      crt->next = new ListNodeBase(*it);
      crt = crt->next;
    }
  }

  /**
   * Advance the pointer by the given number of positions.
   * If the list ends before reaching the desired position, returns nullptr.
   */
  ThisPtr advance(std::size_t positions) {
    ThisPtr l{this};
    while (l && positions > 0) {
      l = l->next;
      --positions;
    }
    return l;
  }

  // Reverse: head -> L1 -> L2 -> L3 -> ... Ln -> tail
  //      To: head -> Ln -> Ln-1 -> ... L2 -> L1 -> tail
  static ThisPtr reverse(ThisPtr head, ThisPtr tail) {
    if (head->next == tail) {
      return head;
    }

    ThisPtr prev{head};
    for (auto it = head->next; it != tail;) {
      auto next = it->next;
      it->next = prev;
      prev = it;
      it = next;
    }
    // Now prev points to Ln and head->next points to L1
    auto l1 = head->next;
    head->next = prev;
    l1->next = tail;

    return l1;
  }

  /**
   * Compare two lists for equality.
   * Returns true if they are equal, false otherwise.
   */
  bool operator==(const ListNodeBase& other) const {
    const ListNodeBase* l1{this};
    const ListNodeBase* l2{&other};
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
  bool operator==(const std::initializer_list<T>& vals) const {
    auto it = vals.begin();
    const ListNodeBase* crt{this};
    for (; it != vals.end() && crt; ++it, crt = crt->next) {
      if (crt->val != *it) {
        return false;
      }
    }
    return it == vals.end() && crt == nullptr;
  }

  /**
   * Unlink the next node and return it. The current node will now point to
   * null as next.
   */
  ListNodeBase* unlink() {
    ListNodeBase* next_node = next;
    next = nullptr;
    return next_node;
  }

  // Destructor to free the entire list with the exception of the head node
  ~ListNodeBase() { delete next; }

  /**
   * Merge two sorted lists
   */
  static ListNodeBase* merge(ListNodeBase* list1, ListNodeBase* list2) {
    return ListNodeBase::merge_sorted_lists(list1, list2);
  }

  /**
   * Merge two sorted lists
   */
  static ListNodeBase* merge_sorted_lists(ListNodeBase* list1,
                                          ListNodeBase* list2) {
    ListNodeBase head;
    ListNodeBase* crt{&head};

    while (list1 || list2) {
      if (list1) {
        if (list2) {
          if (list1->val <= list2->val) {
            crt->next = list1;
            crt = list1;
            list1 = list1->next;
          } else {
            crt->next = list2;
            crt = list2;
            list2 = list2->next;
          }
        } else {
          // List 2 is empty, we can break
          crt->next = list1;
          break;
        }
      } else {
        // List 1 is empty, we can break
        crt->next = list2;
        break;
      }
    }
    return head.unlink();
  }

  ListNodeBase* middle_node() {
    ListNodeBase* slow = this;
    ListNodeBase* fast = this;
    while (fast && fast->next) {
      slow = slow->next;
      fast = fast->next->next;
    }
    return slow;
  }

  /**
   * Split the list into two halves at the middle node.
   * If the list has an even number of nodes, the first half will have one
   * less node than the second half.
   * Returns a pair of pointers to the heads of the two halves.
   */
  std::pair<ListNodeBase*, ListNodeBase*> middle_split() {
    ListNodeBase* middle = nullptr;
    ListNodeBase* slow = this;
    ListNodeBase* fast = this;
    while (fast && fast->next) {
      middle = slow;
      slow = slow->next;
      fast = fast->next->next;
    }

    if (!middle) {
      return {this, nullptr};
    }
    auto next = middle->next;
    middle->next = nullptr;
    return {this, next};
  }

  /**
   * Sort the linked list using merge sort algorithm.
   * Returns the head of the sorted list.
   */
  static ListNodeBase* sort(ListNodeBase* head) {
    // Base case: empty list or single node
    if (!head || !head->next) {
      return head;
    }

    // If there are two nodes, just swap if out of order
    if (!head->next->next) {
      if (head->val > head->next->val) {
        auto second = head->next;
        head->next = second->next;
        second->next = head;
        return second;
      } else {
        return head;
      }
    }

    // Split the list into two halves
    auto [first_half, second_half] = head->middle_split();

    // Recursively sort both halves
    first_half = sort(first_half);
    second_half = sort(second_half);

    // Merge the sorted halves
    return merge_sorted_lists(first_half, second_half);
  }
};

}  // namespace lc_libs

#if __has_include(<iostream>)
#include <iostream>
namespace lc_libs {

template <typename T>
std::ostream& operator<<(std::ostream& os, const ListNodeBase<T>& node) {
  const ListNodeBase<T>* crt = &node;
  os << "[";
  while (crt) {
    os << crt->val;
    crt = crt->next;
    if (crt) os << ", ";
  }
  os << "]";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
                         const typename ListNodeBase<T>::ViewSep& view) {
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
std::ostream& operator<<(std::ostream& os,
                         const typename ListNodeBase<T>::View& view) {
  // View is a 2-tuple (start, end) — delegate to ViewSep with ", "
  return os << typename ListNodeBase<T>::ViewSep{std::get<0>(view),
                                                 std::get<1>(view), ", "};
}

}  // namespace lc_libs
#endif  // __has_include(<iostream>)

#if __has_include(<format>)
#include <format>
#include <tuple>

namespace std {

// Formatter for 3-tuple: (ListNodeBase<T>*, ListNodeBase<T>*, const char*)
template <typename T, typename CharT>
struct std::formatter<::lc_libs::ListView<T>, CharT> {
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ::lc_libs::ListView<T>& view, FormatContext& ctx) const
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
struct std::formatter<::lc_libs::ListNodeBase<T>*, CharT>
    : std::formatter<std::basic_string<CharT>, CharT> {
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ::lc_libs::ListNodeBase<T>* const& node,
              FormatContext& ctx) const -> decltype(ctx.out()) {
    ::lc_libs::ListView<T> sep{node, nullptr, ", "};
    return std::formatter<decltype(sep), CharT>{}.format(sep, ctx);
  }
};

template <typename T, typename CharT>
struct std::formatter<::lc_libs::ListNodeBase<T>, CharT>
    : std::formatter<std::basic_string<CharT>, CharT> {
  constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
      -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ::lc_libs::ListNodeBase<T>& node, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    return std::formatter<::lc_libs::ListNodeBase<T>*, CharT>{}.format(&node,
                                                                       ctx);
  }
};

}  // namespace std
#endif  // __has_include(<format>)