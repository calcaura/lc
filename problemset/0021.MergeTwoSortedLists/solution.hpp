#pragma once

#include <assert.h>

#include <initializer_list>
#include <iostream>
#include <lc_libs/list_node.hpp>

using ListNode = lc_libs::ListNodeBase<int>;

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