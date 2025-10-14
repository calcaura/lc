// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <cmath>
#include <lc_libs/simple_log.hpp>

TEST(Pow_x_n, BasicOperations) {
  for (auto [base, exponent] : std::vector<std::pair<double, int>>{
           {2.0, 10},
           {2.1, 3},
           {2.0, -2},
           {2.0, 0},
           {1.0, std::numeric_limits<int>::min()},
           {1.0, std::numeric_limits<int>::max()},
           {2.0, std::numeric_limits<int>::min()},
           {2.0, std::numeric_limits<int>::max()},
           {0.0, 0},
           {0.0, 10},
           {0.0, -10},
       }) {
    EXPECT_DOUBLE_EQ(Solution::myPow(base, exponent), std::pow(base, exponent))
        << "base: " << base << ", exponent: " << exponent;
  }
}
