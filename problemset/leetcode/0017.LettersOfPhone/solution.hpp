#pragma once

#include <array>
#include <string>
#include <string_view>
#include <vector>

static inline std::string_view letters_of(char c) {
  static std::array<std::string_view, 8> _alphabet{
      "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
  return _alphabet[c - '2'];
}

class Solution {
 public:
  std::vector<std::string> result;

  std::vector<std::string> letterCombinations(std::string_view digits) {
    if (digits.empty()) {
      return {};
    }
    for (char c : letters_of(digits[0])) {
      result.push_back({c});
    }
    for (auto v : digits.substr(1)) {
      auto copy = std::move(result);
      result.clear();
      for (auto letter : letters_of(v)) {
        for (auto &str : copy) {
          result.push_back(str + letter);
        }
      }
    }
    return result;
  }
};