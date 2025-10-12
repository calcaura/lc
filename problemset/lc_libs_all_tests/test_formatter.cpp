
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <lc_libs/formatter.hpp>
#include <lc_libs/simple_log.hpp>
#include <lc_libs/strings.hpp>
#include <lc_libs/tree_node.hpp>
#include <sstream>
#include <vector>

namespace log = lc_libs::log;
using TreeNode = lc_libs::TreeNodeBase<int>;

struct MyStruct {
  int www;
  std::string sss;
};

struct YourStruct {
  size_t x;
  size_t y;
};

namespace std {
template <>
struct formatter<MyStruct>
    : public ::lc_libs::formatter::aligned_formatter<MyStruct, 50> {
  // Formats the value into the context
  template <typename Iterator>
  static inline auto format_unaligned(const MyStruct& s, Iterator out,
                                      size_t max_size) {
    return std::format_to_n(out, max_size, "{} {}", s.www, s.sss);
  }
};

template <>
struct formatter<YourStruct>
    : public ::lc_libs::formatter::aligned_formatter<YourStruct, 10> {
  // Formats the value into the context
  template <typename Iterator>
  static inline auto format_unaligned(const YourStruct& s, Iterator out,
                                      size_t max_size) {
    return std::format_to_n(out, max_size, "{} + {} = {}", s.x, s.y, s.x + s.y);
  }
};
}  // namespace std

TEST(Formatter, BasicOperations) {
  MyStruct s{42, "hello"};  //.
  EXPECT_EQ(std::format("{}", s), "42 hello");
  EXPECT_EQ(std::format("{:>20}", s), "           42 hello");
  EXPECT_EQ(std::format("{:>4}", s), "42 hello");

  EXPECT_EQ(std::format("{:<20}", s), "42 hello           ");
  EXPECT_EQ(std::format("{:<4}", s), "42 hello");

  EXPECT_EQ(std::format("{:^20}", s), "     42 hello      ");
  EXPECT_EQ(std::format("{:^4}", s), "42 hello");

  EXPECT_EQ(std::format("{:.^20}", s), ".....42 hello......");
  EXPECT_EQ(std::format("{:_^20}", s), "_____42 hello______");

  EXPECT_EQ(std::format("{:.^4}", s), "42 hello");
}

TEST(Formatter, Truncated) {
  EXPECT_EQ(std::format("{}", YourStruct{42, 73}),
            "42 + 73 = ");  // Doesn't fit in 10 chars
  EXPECT_EQ(std::format("{}", YourStruct{142, 173}),
            "142 + 173 ");  // Doesn't fit in 10 chars
  EXPECT_EQ(std::format("{}", YourStruct{123456789012, 0}),
            "1234567890");  // Doesn't fit in 10 chars
  EXPECT_EQ(std::format("{}", YourStruct{1, 1}), "1 + 1 = 2");
  EXPECT_EQ(std::format("{:>8}", YourStruct{1, 1}), "1 + 1 = 2");
}
