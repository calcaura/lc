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
