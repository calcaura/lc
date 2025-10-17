#pragma once

#include <cstdint>
#include <deque>
#include <lc_libs/simple_log.hpp>
#include <string>
#include <string_view>

static inline char is_op(std::string_view sv) {
  if (sv.size() != 1) {
    return 0;
  }
  char c = sv[0];

  if (c == '*' || c == '+' || c == '/' || c == '-') {
    return c;
  }
  return 0;
}

static inline int64_t exec_op(int64_t rhs, int64_t lhs, char op) {
  switch (op) {
    case '*':
      return lhs * rhs;
    case '+':
      return lhs + rhs;
    case '/':
      return lhs / rhs;
    case '-':
      return lhs - rhs;
    default:
      DIE_IF(true, "Operation not understood: '{}'", op);
      return -1;
  }
}

static inline int64_t to_number(const char* str) {
  char* err{};
  int64_t result = strtoll(str, &err, 10);
  DIE_IF((err != nullptr && *err != 0), "Value not a number: '{}'", str);
  return result;
}

static inline int64_t to_number(const std::string& str) {
  return to_number(str.c_str());
}

class Solution {
 public:
  static int evalRPN(std::vector<std::string>& tokens) {
    std::deque<int64_t> stack;
    auto pop_last = [&stack]() mutable -> int64_t {
      DIE_IF(stack.empty(), "Failed to extract value from stack");
      int64_t back = std::move(stack.back());
      stack.pop_back();
      return back;
    };
    for (auto& tok : tokens) {
      const char op = is_op(tok);
      if (op == 0) {
        stack.push_back(to_number(tok));
      } else {
        stack.emplace_back(exec_op(pop_last(), pop_last(), op));
      }
    }
    // At this point, the stack must contain exactly one element
    DIE_IF(stack.size() != 1, "Stack ended up with {} elements", stack.size());
    return stack[0];
  }
};