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

TEST(MergeSortedArrayInplace, BaseTest) {
  Solution solution;
  std::vector<int> v1{1, 2, 3, 0, 0, 0};
  std::vector<int> v2{2, 5, 6};
  solution.merge(v1, 3, v2, 3);
  EXPECT_EQ(v1, (std::vector<int>{1, 2, 2, 3, 5, 6}));
}

TEST(MergeSortedArrayInplace, ExtraTest) {
  Solution solution;
  std::vector<int> v1{1};
  std::vector<int> v2{};
  solution.merge(v1, 1, v2, 0);
  EXPECT_EQ(v1, (std::vector<int>{1}));

  v1 = {0};
  v2 = {1};
  solution.merge(v1, 0, v2, 1);
  EXPECT_EQ(v1, (std::vector<int>{1}));

  v1 = {4, 5, 6, 0, 0, 0};
  v2 = {1, 2, 3};
  solution.merge(v1, 3, v2, 3);
  EXPECT_EQ(v1, (std::vector<int>{1, 2, 3, 4, 5, 6}));

  v1 = {2, 0};
  v2 = {1};
  solution.merge(v1, 1, v2, 1);
  EXPECT_EQ(v1, (std::vector<int>{1, 2}));
}
