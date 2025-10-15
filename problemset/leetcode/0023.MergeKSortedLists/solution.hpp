#pragma once

#include <lc_libs/list_node.hpp>
#include <queue>
#include <vector>

using ListNode = lc_libs::ListNodeBase<int>;

static inline bool ListNodeLess(const ListNode* a, const ListNode* b) {
  return a->val > b->val;
}

/**
 * Merge two lists using a priority queue (suboptimal)
 */
static inline ListNode* mergeKListsPQ(std::vector<ListNode*>& lists) {
  if (lists.empty()) {
    return nullptr;
  }
  if (lists.size() == 2) {
    return ListNode::merge(lists[0], lists[1]);
  }

  std::priority_queue<ListNode*, std::vector<ListNode*>,
                      decltype(&ListNodeLess)>
      queue(&ListNodeLess);

  for (ListNode* list : lists) {
#ifdef DEBUG
    std::cout << "Input: " << *list << std::endl;
#endif
    if (list) {
      queue.push(list);
    }
  }

  if (queue.empty()) {
    return {};
  }

  ListNode* head{};
  ListNode* crt{};

  while (!queue.empty()) {
    auto next = queue.top();
    queue.pop();

    if (next->next) {
#ifdef DEBUG
      std::cout << "  Enq: " << next->next->val << std::endl;
#endif
      queue.push(next->next);
    }

    if (head == nullptr) {
      head = crt = next;
    } else {
      crt->next = next;
      crt = next;
    }
#ifdef DEBUG
    crt->next = nullptr;  // debug
    std::cout << "Popped: " << next->val << ", Result: " << *head << std::endl;
#endif
  }
  return head;
}

/**
 * Merge K List by bisecting the merges.
 * This is faster than usig a priority queue.
 */
static inline ListNode* mergeKListsBisect(std::vector<ListNode*>& lists,
                                          std::size_t start, std::size_t end) {
  if (start == end) {
    return {};
  }
  if ((end - start) == 1) {
    return lists[start];
  }

  if ((end - start) == 2) {
    return ListNode::merge(lists[start], lists[start + 1]);
  }

  std::size_t mid = (start + end) / 2;

  auto left = mergeKListsBisect(lists, start, mid);
  auto right = mergeKListsBisect(lists, mid, end);
  return ListNode::merge(left, right);
}

class Solution {
 public:
  ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    // return mergeKListsPQ(lists);

    return mergeKListsBisect(lists, 0, lists.size());
  }
};