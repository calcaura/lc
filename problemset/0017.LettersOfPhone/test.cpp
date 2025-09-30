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

static inline void test(std::string_view in,
                        std::initializer_list<std::string> out) {
  Solution solution;
  auto result = solution.letterCombinations(in);
  std::sort(result.begin(), result.end());
  std::vector<std::string> expected(out);
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(result, expected);
}

TEST(TemplateNameTest, BasicOperations) {
  Solution solution;

  test("23", {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"});
  test("", {});
  test("2", {"a", "b", "c"});
  test("946", {"wgm", "wgn", "wgo", "whm", "whn", "who", "wim", "win", "wio",
               "xgm", "xgn", "xgo", "xhm", "xhn", "xho", "xim", "xin", "xio",
               "ygm", "ygn", "ygo", "yhm", "yhn", "yho", "yim", "yin", "yio",
               "zgm", "zgn", "zgo", "zhm", "zhn", "zho", "zim", "zin", "zio"});
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}