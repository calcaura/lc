// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <any>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <lc_libs/trie.hpp>
#include <vector>

namespace log = lc_libs::log;

std::vector<std::any> ops(const std::vector<std::string>& op,
                          const std::vector<std::string>& values) {
  lc_libs::Trie trie;
  std::vector<std::any> results;
  for (size_t i = 0; i < op.size(); ++i) {
    if (op[i] == "insert") {
      trie.insert(values[i].c_str());
      results.push_back(nullptr);
    } else if (op[i] == "search") {
      results.push_back(trie.search(values[i].c_str()));
    } else if (op[i] == "startsWith") {
      results.push_back(trie.startsWith(values[i].c_str()));
    } else if (op[i] == "remove") {
      results.push_back(trie.remove(values[i].c_str()));
    } else {
      throw std::runtime_error("Unknown operation: " + op[i]);
    }
  }
  return results;
}

TEST(ImplementTrie, BasicOperations) {
  std::vector<std::string> ops_vec{"insert",     "insert", "search", "search",
                                   "startsWith", "insert", "search"};
  std::vector<std::string> values_vec{"apple", "app", "apple", "app",
                                      "app",   "app", "app"};
  std::vector<std::any> expected{nullptr, nullptr, true, true,
                                 true,    nullptr, true};

  auto results = ops(ops_vec, values_vec);

  ASSERT_EQ(results.size(), expected.size());
  for (size_t i = 0; i < expected.size(); ++i) {
    EXPECT_TRUE(results[i].type() == expected[i].type()) << " at index " << i;

    if (expected[i].type() == typeid(std::nullptr_t)) {
      EXPECT_EQ(std::any_cast<std::nullptr_t>(results[i]),
                std::any_cast<std::nullptr_t>(expected[i]));
    } else if (expected[i].type() == typeid(bool)) {
      EXPECT_EQ(std::any_cast<bool>(results[i]),
                std::any_cast<bool>(expected[i]));
    } else {
      FAIL() << "Unexpected type in results";
    }
  }
}

TEST(ImplementTrie, WordList) {
  lc_libs::Trie trie;
  std::vector<std::string> words{"hello", "hell", "heaven", "heavy"};
  trie.insert(words);

  for (const auto& word : words) {
    EXPECT_TRUE(trie.search(word.c_str())) << "Failed to find word: " << word;
  }

  auto words_in_trie = trie.get_all_words();
  std::sort(words.begin(), words.end());
  EXPECT_EQ(words_in_trie, words);

  EXPECT_TRUE(trie.remove("hell"));
  EXPECT_FALSE(trie.remove("hell"));
  EXPECT_FALSE(trie.search("hell"));
  EXPECT_TRUE(trie.search("hello"));
  EXPECT_TRUE(trie.remove("hello"));
  EXPECT_FALSE(trie.search("hello"));
  EXPECT_TRUE(trie.search("heaven"));
  EXPECT_TRUE(trie.search("heavy"));
}

void erase(std::vector<std::string>& vec,
           std::initializer_list<std::string> values) {
  for (const auto& value : values) {
    vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
  }
}

TEST(ImplementTrie, WordListInternal) {
  lc_libs::Trie trie;
  std::vector<std::string> words{"hello", "hell", "heaven", "heavy"};
  trie.insert(words);

  auto words_in_trie = trie.get_all_words(true);
  std::sort(words.begin(), words.end());

  std::vector<std::string> internal_words{"h",     "he",      "hea",    "heav",
                                          "heave", "heaven$", "heavy$", "hel",
                                          "hell",  "hell$",   "hello$"};
  EXPECT_EQ(words_in_trie, internal_words);

  // Removing the word hell. The end word marker is removed, however the branch
  // still remains because it is a prefix for hello.
  EXPECT_TRUE(trie.remove("hell"));
  erase(internal_words, {"hell$"});

  EXPECT_EQ(trie.get_all_words(true), internal_words);

  // Trying to remove hell again should fail
  EXPECT_FALSE(trie.remove("hell"));
  EXPECT_EQ(trie.get_all_words(true), internal_words);

  // Trying to search hell again should fail.
  EXPECT_FALSE(trie.search("hell"));

  EXPECT_TRUE(trie.search("hello"));
  EXPECT_TRUE(trie.remove("hello"));
  // Now hello, hell, hel should be removed from the internal words.
  erase(internal_words, {"hello$", "hell", "hel"});
  EXPECT_EQ(trie.get_all_words(true), internal_words);
  EXPECT_FALSE(trie.search("hello"));

  // Erase heaven
  EXPECT_TRUE(trie.search("heaven"));
  EXPECT_TRUE(trie.remove("heaven"));
  erase(internal_words, {"heaven$", "heave"});
  EXPECT_EQ(trie.get_all_words(true), internal_words);

  // Final erasure
  EXPECT_TRUE(trie.search("heavy"));

  EXPECT_TRUE(trie.remove("heavy"));
  erase(internal_words, {"heavy$", "heav", "hea", "he", "h"});
  EXPECT_EQ(trie.get_all_words(true), internal_words);
}