// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <vector>

namespace log = lc_libs::log;

TEST(ListNode, Printer) {
  ListNode list{1, 2, 3, 4, 5};
  EXPECT_EQ(std::format("[{}]", list), "[1, 2, 3, 4, 5]");

  auto adv = list.advance(2);
  EXPECT_EQ(std::format("[{}]", adv), "[3, 4, 5]");

  EXPECT_EQ(std::format("[{}]", list.view("->")), "[1->2->3->4->5]");
  auto view = ListNode::view(&list, list.advance(3), "|");
  EXPECT_EQ(std::format("/{}/", view), "/1|2|3|4/");
}

TEST(MergeKSortedLists, Null) {
  Solution s;
  std::vector<ListNode*> lists;
  EXPECT_EQ(s.mergeKLists(lists), nullptr);
}

TEST(MergeKSortedLists, ThreeLists) {
  Solution s;
  std::vector<ListNode*> lists;
  lists.push_back(new ListNode({1, 4, 5}));
  lists.push_back(new ListNode({1, 3, 4}));
  lists.push_back(new ListNode({2, 6}));

  log::debug("Merging lists: [{}]; [{}]; [{}]", lists[0], lists[1], lists[2]);

  ListNode* result = s.mergeKLists(lists);
  EXPECT_EQ(*result, (ListNode{1, 1, 2, 3, 4, 4, 5, 6}));
  delete result;
}