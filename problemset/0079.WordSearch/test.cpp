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

TEST(WordSearch, BasicOperations) {
  std::vector<std::vector<char>> board{
      {'A', 'B', 'C', 'E'}, {'S', 'F', 'C', 'S'}, {'A', 'D', 'E', 'E'}};

  Searcher searcher(board);
  log::debug("Searcher:\n{}", searcher);
  EXPECT_TRUE(searcher.exists("ABCCED"));
  EXPECT_TRUE(searcher.exists("SEE"));
  EXPECT_FALSE(searcher.exists("ABCB"));
}
