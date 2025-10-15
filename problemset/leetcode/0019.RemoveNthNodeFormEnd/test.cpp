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

TEST(TemplateNameTest, BasicOperations) {
  Solution solution;
  ListNode* input = new ListNode({1, 2, 3, 4, 5});
  auto [result, removed] = solution.removeNthFromEnd(std::move(input), 2);
  ASSERT_NE(removed, nullptr);
  ASSERT_EQ(*removed, (ListNode({4})));
  ASSERT_EQ(*result, (ListNode({1, 2, 3, 5})));
}
