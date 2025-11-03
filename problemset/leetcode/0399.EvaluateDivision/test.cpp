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

TEST(EvaluateDivision, tc1) {
  std::vector<std::vector<std::string>> equations{{"a", "b"}, {"b", "c"}};
  std::vector<double> values{2.0, 3.0};
  std::vector<std::vector<std::string>> queries{
      {"a", "c"}, {"b", "a"}, {"a", "e"}, {"a", "a"}, {"x", "x"}};
  std::vector<double> expected{6.0, 0.5, -1.0, 1.0, -1.0};
  auto result = Solution::calcEquation(equations, values, queries);
  EXPECT_EQ(result, expected);
}

TEST(EvaluateDivision, tc2) {
  std::vector<std::vector<std::string>> equations{
      {"a", "b"}, {"b", "c"}, {"bc", "cd"}};
  std::vector<double> values{1.5, 2.5, 5.0};
  std::vector<std::vector<std::string>> queries{
      {"a", "c"}, {"c", "b"}, {"bc", "cd"}, {"cd", "bc"}};
  std::vector<double> expected{3.75, 0.4, 5.0, 0.2};
  auto result = Solution::calcEquation(equations, values, queries);
  EXPECT_EQ(result, expected);
}

TEST(EvaluateDivision, tc3) {
  std::vector<std::vector<std::string>> equations{{"a", "b"}};
  std::vector<double> values{0.5};
  std::vector<std::vector<std::string>> queries{
      {"a", "b"}, {"b", "a"}, {"a", "c"}, {"x", "y"}};
  std::vector<double> expected{0.5, 2.0, -1.0, -1.0};
  auto result = Solution::calcEquation(equations, values, queries);
  EXPECT_EQ(result, expected);
}