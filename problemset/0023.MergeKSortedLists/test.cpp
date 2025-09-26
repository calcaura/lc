// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <vector>

TEST(TemplateNameTest, Null) {
  Solution s;
  std::vector<ListNode*> lists;
  EXPECT_EQ(s.mergeKLists(lists), nullptr);
}

TEST(TemplateNameTest, ThreeLists) {
  Solution s;
  std::vector<ListNode*> lists;
  lists.push_back(new ListNode({1, 4, 5}));
  lists.push_back(new ListNode({1, 3, 4}));
  lists.push_back(new ListNode({2, 6}));
  ListNode* result = s.mergeKLists(lists);
  EXPECT_EQ(*result, (ListNode{1, 1, 2, 3, 4, 4, 5, 6}));
  delete result;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}