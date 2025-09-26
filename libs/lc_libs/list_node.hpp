#pragma once
#include <initializer_list>

namespace lc_libs {

template <typename T>
struct ListNodeBase {
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
};
}  // namespace lc_libs