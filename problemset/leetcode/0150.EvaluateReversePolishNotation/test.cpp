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

TEST(EvaluateReversePolishNotation, BasicOperations) {
  {
    std::vector<std::string> tokens{"2", "1", "+", "3", "*"};
    EXPECT_EQ(Solution::evalRPN(tokens), 9);
  }
  {
    std::vector<std::string> tokens{"4", "13", "5", "/", "+"};
    EXPECT_EQ(Solution::evalRPN(tokens), 6);
  }
  {
    std::vector<std::string> tokens{"10", "6", "9",  "3", "+", "-11", "*",
                                    "/",  "*", "17", "+", "5", "+"};
    EXPECT_EQ(Solution::evalRPN(tokens), 22);
  }
}
