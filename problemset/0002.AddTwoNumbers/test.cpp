
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

TEST(AddTwoNumbers, Base) {
  Solution solution;
  ListNode l1{2, 4, 3};
  ListNode l2{5, 6, 4};
  ListNode expected{7, 0, 8};
  auto result = solution.addTwoNumbers(&l1, &l2);
  EXPECT_TRUE(*result == expected);
}

TEST(AddTwoNumbers, Large) {
  Solution solution;
  ListNode l1{9, 9, 9, 9, 9, 9, 9};
  ListNode l2{9, 9, 9, 9};
  ListNode expected{8, 9, 9, 9, 0, 0, 0, 1};
  auto result = solution.addTwoNumbers(&l1, &l2);
  EXPECT_TRUE(*result == expected);
}