#pragma once

#include <lc_libs/list_node.hpp>

using ListNode = lc_libs::ListNodeBase<int>;
class Solution {
 public:
  template <typename R = std::unique_ptr<ListNode>>
  R reverseKGroup(ListNode* l, int k) {
    if (k < 2) {
      return R{l};
    }
    ListNode head{0, l};
    ListNode* crt{&head};

    for (;;) {
      ListNode* tail = crt->advance(k);
      if (tail == nullptr) {
        break;
      }
      crt = ListNode::reverse(crt, tail->next);
    }
    return R{head.unlink()};
  }
};