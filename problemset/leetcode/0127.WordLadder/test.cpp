// Must be included first
#include "solution2.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <vector>

namespace log = lc_libs::log;

TEST(WordLadder, BasicOperations) {
  ASSERT_EQ(Solution::ladderLength("hit", "cog",
                                   {"hot", "dot", "dog", "lot", "log", "cog"}),
            5);
  ASSERT_EQ(
      Solution::ladderLength("hit", "cog", {"hot", "dot", "dog", "lot", "log"}),
      0);

  ASSERT_EQ(Solution::ladderLength("a", "c", {"a", "b", "c"}), 2);

  ASSERT_EQ(Solution::ladderLength("hot", "dog", {"hot", "dog"}), 0);

  ASSERT_EQ(Solution::ladderLength("hot", "dog", {"hot", "dot", "dog"}), 3);
}
