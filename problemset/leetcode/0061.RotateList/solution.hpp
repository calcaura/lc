#pragma once

#include <lc_libs/list_node.hpp>

using ListNode = lc_libs::ListNodeBase<int>;

/**
 * Calculate the length of the list. Return it along with the tail.
 */
static inline std::pair<size_t, ListNode*> listlen(ListNode* head) {
  size_t result{};
  for (;;) {
    ++result;
    if (!head->next) {
      return {result, head};
    }
    head = head->next;
  }
}

class Solution {
 public:
  static ListNode* rotateRight(ListNode* head, int k) {
    if (!head) {
      return head;
    }

    auto [size, tail] = listlen(head);

    // Eliminate redundant rotations
    k %= size;

    // If nothing to rotate, just terminate early.
    if (k == 0) {
      return head;
    }

    // Make the list circular (will break later)
    tail->next = head;

    // Find node at size - k, break the link and return it as head
    k = size - k;
    for (; k > 1; --k) {
      head = head->next;
    }
    auto next = head->next;

    // Finally, break the loop
    head->next = nullptr;
    return next;
  }
};