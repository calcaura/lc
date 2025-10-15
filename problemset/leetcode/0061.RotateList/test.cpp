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

TEST(RotateList, BasicOperations) {
  auto rotate = [](std::initializer_list<int> value, int k) {
    auto list = new ListNode(value);
    return std::unique_ptr<ListNode>(Solution::rotateRight(list, k));
  };

  ASSERT_EQ(*rotate({1, 2, 3, 4, 5}, 2),
            (std::initializer_list<int>{4, 5, 1, 2, 3}));

  ASSERT_EQ(*rotate({0, 1, 2}, 4), (std::initializer_list<int>{2, 0, 1}));
}
