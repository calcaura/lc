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

TEST(DesignAddandSearchWordsDataStructure, BasicOperations) {
  WordDictionary wd;

  wd.addWord("bad");
  wd.addWord("dad");
  wd.addWord("mad");

  EXPECT_FALSE(wd.search("pad"));  // return False
  EXPECT_TRUE(wd.search("bad"));   // return True
  EXPECT_TRUE(wd.search(".ad"));   // return True
  EXPECT_TRUE(wd.search("b.."));   // return True
}
