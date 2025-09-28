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
  EXPECT_EQ(solution.intToRoman(3), "III");
  EXPECT_EQ(solution.intToRoman(4), "IV");
  EXPECT_EQ(solution.intToRoman(9), "IX");
  EXPECT_EQ(solution.intToRoman(58), "LVIII");
  EXPECT_EQ(solution.intToRoman(1994), "MCMXCIV");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}