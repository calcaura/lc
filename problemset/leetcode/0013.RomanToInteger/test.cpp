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

  EXPECT_EQ(solution.romanToInt("III"), 3);
  EXPECT_EQ(solution.romanToInt("IV"), 4);
  EXPECT_EQ(solution.romanToInt("IX"), 9);
  EXPECT_EQ(solution.romanToInt("LVIII"), 58);
  EXPECT_EQ(solution.romanToInt("MCMXCIV"), 1994);
}
