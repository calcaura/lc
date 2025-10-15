// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <format>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <vector>

namespace log = lc_libs::log;

TEST(ReverseNodesKGroup, Reverse2) {
  Solution solution;
  auto l = new ListNode({1, 2, 3, 4, 5});
  auto result = solution.reverseKGroup(l, 2);
  EXPECT_EQ(*result, (ListNode{2, 1, 4, 3, 5}));
}

TEST(ReverseNodesKGroup, Reverse3) {
  Solution solution;
  auto l = new ListNode({1, 2, 3, 4, 5});
  auto result = solution.reverseKGroup(l, 3);
  EXPECT_EQ(*result, (ListNode{3, 2, 1, 4, 5}));
}
