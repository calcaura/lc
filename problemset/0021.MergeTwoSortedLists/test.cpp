// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <vector>

TEST(TemplateNameTest, BasicOperations) {
  Solution solution;
  {
    auto result = solution.mergeTwoLists(nullptr, nullptr);
    EXPECT_EQ(result, nullptr);
  }
  {
    auto list1 = new ListNode{1, 2, 7, 8};
    auto list2 = new ListNode{3, 5, 9};
    auto result =
        std::unique_ptr<ListNode>(solution.mergeTwoLists(list1, list2));
    EXPECT_EQ(*result, ListNode({1, 2, 3, 5, 7, 8, 9}));
  }
}