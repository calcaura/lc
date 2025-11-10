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
using Solution = lc_libs::Solution;

TEST(WordSearchII, NoSol) {
  Solution sol;

  std::vector<std::vector<char>> board{{'a', 'b'}, {'c', 'd'}};

  std::vector<std::string> words{"abcd"};

  std::vector<std::string> expected{};
  std::vector<std::string> result = sol.findWords(board, words);
  std::sort(result.begin(), result.end());
  EXPECT_EQ(result, expected);
}

TEST(WordSearchII, Trivial) {
  Solution sol;

  std::vector<std::vector<char>> board{{'a'}};

  std::vector<std::string> words{"a"};

  std::vector<std::string> expected{"a"};
  std::vector<std::string> result = sol.findWords(board, words);
  std::sort(result.begin(), result.end());
  EXPECT_EQ(result, expected);
}

TEST(WordSearchII, BasicOperations) {
  Solution sol;

  std::vector<std::vector<char>> board{{'o', 'a', 'a', 'n'},
                                       {'e', 't', 'a', 'e'},
                                       {'i', 'h', 'k', 'r'},
                                       {'i', 'f', 'l', 'v'}};

  std::vector<std::string> words{"oath", "pea", "eat", "rain"};

  std::vector<std::string> expected{"eat", "oath"};
  std::vector<std::string> result = sol.findWords(board, words);
  std::sort(result.begin(), result.end());
  EXPECT_EQ(result, expected);
}

TEST(WordSearchII, Long) {
  std::vector<std::vector<char>> board{
      {'d', 'c', 'e', 'b', 'd', 'e', 'd', 'a'},
      {'c', 'a', 'e', 'a', 'd', 'd', 'e', 'e'},
      {'a', 'c', 'e', 'd', 'b', 'c', 'c', 'b'},
      {'c', 'b', 'a', 'a', 'a', 'e', 'e', 'e'},
      {'a', 'e', 'd', 'e', 'b', 'd', 'd', 'e'},
      {'a', 'a', 'd', 'c', 'e', 'a', 'd', 'e'},
      {'b', 'd', 'e', 'b', 'b', 'b', 'c', 'e'},
      {'d', 'a', 'e', 'e', 'b', 'e', 'b', 'd'},
      {'b', 'b', 'c', 'a', 'b', 'b', 'b', 'a'},
      {'a', 'c', 'b', 'a', 'c', 'a', 'd', 'd'}};

  std::vector<std::string> words{
      "ab",      "bddbebcba", "ededa", "daebeda",    "edecaeabc", "cbeedad",
      "bcaaecb", "c",         "eb",    "aadbdbacee", "dcaaba"};

  std::vector<std::string> expected{"c", "eb", "ededa", "ab", "daebeda"};
  Solution sol;
  std::vector<std::string> result = sol.findWords(board, words);

  std::sort(result.begin(), result.end());
  std::sort(expected.begin(), expected.end());

  EXPECT_EQ(result, expected);
}