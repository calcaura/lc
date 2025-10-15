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
  Solution s;

  std::vector<int> v{4, 5, 6, 7, 0, 1, 2};
  EXPECT_EQ(s.search(v, 0), 4);
  EXPECT_EQ(s.search(v, 3), -1);
  EXPECT_EQ(s.search(v, 6), 2);

  v = {1};
  EXPECT_EQ(s.search(v, 0), -1);

  v = {1, 3};
  EXPECT_EQ(s.search(v, 3), 1);

  v = {3, 1};
  EXPECT_EQ(s.search(v, 1), 1);

  v = {5, 1, 3};
  EXPECT_EQ(s.search(v, 5), 0);
}
