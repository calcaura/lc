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
  log::debug("Debug message: {}", 42);
  log::info("Info message: {}", 3.14);
  log::warn("Warning message: {}", "be careful!");
  log::error("Error message: {}", "something went wrong!");
  ASSERT_EQ(1, 1);
  ASSERT_GE(2, 1);
  ASSERT_LE(1, 2);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}