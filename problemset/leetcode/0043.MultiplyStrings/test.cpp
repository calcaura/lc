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

TEST(MultiplyStrings, BasicOperations) {

  Solution s;
  EXPECT_EQ(s.multiply("2", "3"), "6");
  EXPECT_EQ(s.multiply("123", "456"), "56088");
  EXPECT_EQ(s.multiply("0", "0"), "0");
  EXPECT_EQ(s.multiply("9133", "0"), "0");
  EXPECT_EQ(s.multiply("9133", "1"), "9133");
  EXPECT_EQ(s.multiply("9", "9"), "81");
  EXPECT_EQ(s.multiply("99", "99"), "9801");
  EXPECT_EQ(s.multiply("123456789", "987654321"), "121932631112635269");
  EXPECT_EQ(s.multiply("12345678901234567890", "98765432109876543210"),
            "1219326311370217952237463801111263526900");
}
