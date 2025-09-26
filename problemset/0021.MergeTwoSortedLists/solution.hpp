#pragma once

#include <assert.h>

#include <initializer_list>
#include <iostream>
#include <lc_libs/list_node.hpp>

using ListNode = lc_libs::ListNodeBase<int>;

class Solution {
 public:
  ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    return ListNode::merge(list1, list2);
  }
};