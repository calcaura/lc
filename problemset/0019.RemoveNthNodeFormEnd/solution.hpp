#pragma once

#include <lc_libs/list_node.hpp>

using ListNode = lc_libs::ListNodeBase<int>;

class Solution {
 public:
  template <typename L = std::unique_ptr<ListNode>,
            typename R = std::unique_ptr<ListNode>>
  std::pair<L, R> removeNthFromEnd(ListNode* head, int n) {
    ListNode sentinel{0, head};
    ListNode* crt = &sentinel;
    ListNode* rem = crt;
    ListNode* removed{};

    while (n > 0 && crt) {
      crt = crt->next;
      --n;
    }

    if (n > 0 || crt == nullptr) {
      // n is larger than the list size, nothing to remove
      return {L(head), R(removed)};
    }

    while (crt && crt->next) {
      rem = rem->next;
      crt = crt->next;
    }

    if (rem && rem->next) {
      removed = rem->next;
      rem->next = rem->next->next;
      removed->next = nullptr;
    }

    return {L(sentinel.unlink()), R(removed)};
  }
};