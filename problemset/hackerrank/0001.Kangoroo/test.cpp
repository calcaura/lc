// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <lc_libs/simple_log.hpp>

TEST(Kangoroo, BasicOperations) {
  EXPECT_EQ(kangaroo(0, 3, 4, 2), "YES");
  EXPECT_EQ(kangaroo(0, 2, 5, 3), "NO");
  EXPECT_EQ(kangaroo(21, 6, 47, 3), "NO");
  EXPECT_EQ(kangaroo(14, 4, 98, 2), "YES");
  EXPECT_EQ(kangaroo(43, 2, 70, 2), "NO");
}
