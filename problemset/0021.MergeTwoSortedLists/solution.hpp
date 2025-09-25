#pragma once

#include <assert.h>

#include <initializer_list>
#include <iostream>

struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
  ListNode(std::initializer_list<int> vals) : next(nullptr) {
    if (vals.size() == 0) {
      // Empty list
      val = 0;
      next = nullptr;
      return;
    }
    auto it = vals.begin();
    val = *it;
    ++it;
    ListNode* crt{this};
    for (; it != vals.end(); ++it) {
      crt->next = new ListNode(*it);
      crt = crt->next;
    }
  }

  bool operator==(const ListNode& other) const {
    const ListNode* l1{this};
    const ListNode* l2{&other};
    while (l1 && l2) {
      if (l1->val != l2->val) {
        return false;
      }
      l1 = l1->next;
      l2 = l2->next;
    }
    return l1 == nullptr && l2 == nullptr;
  }

  bool operator==(const std::initializer_list<int>& vals) const {
    auto it = vals.begin();
    const ListNode* crt{this};
    for (; it != vals.end() && crt; ++it, crt = crt->next) {
      if (crt->val != *it) {
        return false;
      }
    }
    return it == vals.end() && crt == nullptr;
  }

  ListNode* unlink() {
    ListNode* next_node = next;
    next = nullptr;
    return next_node;
  }

  // Destructor to free the entire list with the exception of the head node
  ~ListNode() { delete next; }
};

class Solution {
 public:
  ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode head;
    ListNode* crt{&head};

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
};