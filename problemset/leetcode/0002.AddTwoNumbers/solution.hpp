#pragma once

#include <lc_libs/list_node.hpp>

using ListNode = lc_libs::ListNodeBase<int>;

class Solution {
 public:
  template <typename R = std::unique_ptr<ListNode>>
  R addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode head;
    int carry = 0;
    ListNode* current = &head;

    while (l1 || l2 || carry) {
      if (l1) {
        carry += l1->val;
        l1 = l1->next;
      }
      if (l2) {
        carry += l2->val;
        l2 = l2->next;
      }

      ListNode* next = new ListNode(carry % 10);
      current->next = next;
      current = next;
      carry /= 10;
    }

    return R{head.unlink()};
  }
};
