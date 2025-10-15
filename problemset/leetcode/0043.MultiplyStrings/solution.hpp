#pragma once

#include <string>
#include <string_view>

/**
 * Convert in-place from ascii to int: '0' .. '9' -> 0 .. 9.
 * Strip all leading '0' and return a view on the string.
 */
static inline std::string_view normalize(std::string& sv) {
  size_t start{};
  for (; start < sv.size(); ++start) {
    if (sv[start] != '0') {
      break;
    }
  }
  for (size_t it = start; it < sv.size(); ++it) {
    sv[it] -= '0';
  }
  return std::string_view(&sv[start], sv.size() - start);
}

/**
 * Convert in-place from int to ascii: 0 .. 9 -> '0' .. '9'
 */
static inline void as_ascii(std::string& str) {
  for (auto& v : str) {
    v += '0';
  }
}

/**
 * Multiply a large number with a 1-digit number then add it to the output as-if
 * it was shifted to left. E.g
 *   ```mul_add("00100", "12", '2', 3)```
 *  will do:
 *   `12 * 2 => 24`
 *   `24 << 3 => 24000`
 *   `00100 + 24000 => 24100`
 * Hence, at the exit of this function `out = 24100`
 */
static inline void mul_add(std::string& out, std::string_view lh,
                           unsigned char c, size_t shift) {
  size_t carry{};
  auto insert = out.rbegin() + shift;
  for (auto it = lh.rbegin(); it != lh.rend(); ++it, ++insert) {
    auto val = *it * c;
    val += carry;
    val += *insert;
    *insert = val % 10;
    carry = val / 10;
  }
  while (carry) {
    auto val = *insert + carry;
    *insert = val % 10;
    carry = val / 10;
  }
}

/**
 * Remove the leading zeros from potentially overprovisioned string.
 */
static inline void remove_leading_zeros(std::string& str) {
  size_t sz{};
  for (auto c : str) {
    if (c != 0) {
      if (sz) str.erase(0, sz);
      return;
    }
    ++sz;
  }
}

class Solution {
 public:
  static std::string multiply(const char* lh, const char* rh) {
    std::string lsh(lh);
    std::string rsh(rh);
    return multiply(lsh, rsh);
  }

  // Multiply two large decimal numbers
  static std::string multiply(std::string& lh, std::string& rh) {
    auto num1 = normalize(lh);
    auto num2 = normalize(rh);

    if (num1.empty() || num2.empty()) {
      return "0";
    }

    // Preparing the result object.
    // Reserve & preallocate the max number of output digits
    // which in the worst case scenario is n + m + 1.
    // E.g. 999 * 999 = 1'009'101
    const std::string::size_type max_digits = num1.size() + num2.size() + 1;

    // Pre-fill the result with zeros
    std::string result(max_digits, static_cast<char>('\0'));

    // How many positions should be the second sum shifted to the left
    // before addition
    size_t lshift{};

    for (auto rhit = num2.rbegin(); rhit != num2.rend(); ++rhit, ++lshift) {
      mul_add(result, num1, *rhit, lshift);
    }

    // If the input numbers were not all '9's then it's guaranteed to have
    // leading zeros
    remove_leading_zeros(result);

    // Convert the number back to printable
    as_ascii(result);

    return result;
  }
};