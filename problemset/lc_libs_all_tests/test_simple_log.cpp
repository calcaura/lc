
#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <lc_libs/strings.hpp>
#include <lc_libs/tree_node.hpp>
#include <sstream>
#include <vector>

namespace log = lc_libs::log;
using TreeNode = lc_libs::TreeNodeBase<int>;

TEST(SimpleLoggerTest, BasicOperations) {
  std::stringstream ss;
  {
    log::OutputGuard guard(ss);
    log::LogLevelGuard level_guard(log::LogLevel::Debug);
    log::debug("Hello, {}!", "world");
    log::info("The answer is {}.", 42);
    log::warn("This is a warning.");
    log::error("This is an error.");
  }
  std::string output = ss.str();
  // Check that the output contains the expected log messages
  EXPECT_NE(output.find("Hello, world!"), std::string::npos);
  EXPECT_NE(output.find("The answer is 42."), std::string::npos);
  EXPECT_NE(output.find("This is a warning."), std::string::npos);
  EXPECT_NE(output.find("This is an error."), std::string::npos);
}

TEST(SimpleLoggerTest, IndentedOperations) {
  std::stringstream ss;
  auto guard = log::OutputGuard(ss);
  log::LogLevelGuard level_guard(log::LogLevel::Debug);
  auto recurse = [](int n, const auto& recurse_ref) -> void {
    log::LogIndentGuard indent_guard;
    if (n == 0) {
      log::warn("Base case reached.");
      return;
    }
    log::info("Recursing with n = {}.", n);
    log::debug("Debug info at n = {}.", n);
    recurse_ref(n - 1, recurse_ref);
    log::info("Returning from n = {}.", n);
  };
  recurse(3, recurse);
  std::string output = ss.str();
  std::vector<std::string_view> lines =
      lc_libs::strings::split<std::vector<std::string_view>>(output, '\n');

  // Check that the output contains the expected log messages
  std::vector<std::string_view> expected_lines = {
      " [inf] -  Recursing with n = 3.",    //
      " [dbg] -  Debug info at n = 3.",     //
      " [inf] -   Recursing with n = 2.",   //
      " [dbg] -   Debug info at n = 2.",    //
      " [inf] -    Recursing with n = 1.",  //
      " [dbg] -    Debug info at n = 1.",   //
      " [wrn] -     Base case reached.",    //
      " [inf] -    Returning from n = 1.",  //
      " [inf] -   Returning from n = 2.",   //
      " [inf] -  Returning from n = 3.",    //
      "",                                   //
  };

  ASSERT_EQ(lines.size(), expected_lines.size());

  for (size_t i = 0; i < lines.size(); ++i) {
    auto line = lines[i];
    auto expected_line = expected_lines[i];
    // Check that the line ends with the expected content (ignoring prefix)
    EXPECT_TRUE(line.ends_with(expected_line))
        << "Line " << i << " does not match. Got: '" << line
        << "', expected ending '" << expected_line << "'";
  }
}

TEST(SimpleLoggerTest, TimeProviderGuard) {
  std::stringstream ss;
  auto guard = log::OutputGuard(ss);
  log::LogLevelGuard level_guard(log::LogLevel::Debug);

  using namespace std::chrono_literals;
  auto custom_time_provider =
      []() -> std::chrono::time_point<std::chrono::system_clock> {
    return std::chrono::system_clock::time_point(420ms);
  };

  {
    log::TimeProviderGuard time_guard(custom_time_provider);
    log::info("This log uses the custom time provider.");
  }

  log::error("This log uses the default time provider.");

  std::string output = ss.str();
  std::vector<std::string_view> lines =
      lc_libs::strings::split<std::vector<std::string_view>>(output, '\n');

  // Check that the output contains two log messages
  ASSERT_EQ(lines.size(), 3);  // Including the trailing newline

  // Check that the first log message was generated with the custom time
  EXPECT_EQ(lines[0],
            "1970-01-01T00:00:00.420000000 [inf] - This log uses the custom "
            "time provider.");
  // Check that the second log message was generated with the default time
  EXPECT_NE(lines[1].find(" [err] - This log uses the default time provider."),
            std::string::npos);
}