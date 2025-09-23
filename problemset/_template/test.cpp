// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <vector>

TEST(TemplateNameTest, BasicOperations) {
  ASSERT_EQ(1, 1);
  ASSERT_GE(2, 1);
  ASSERT_LE(1, 2);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}