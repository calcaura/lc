#pragma once

#include <cstddef>
#include <limits>

class Solution {
 public:
  static double myPow(double x, int n) {
    if (x == 1) {
      return 1;
    }

    if (n < 0) {
      if (n == std::numeric_limits<int>::min()) {
        return 1 / pow(x, -(n + 1)) / x;
      }
      return 1 / pow(x, -n);
    }
    return pow(x, n);
  }

  static double pow(double a, size_t n) {
    double result{1};
    while (n) {
      if (n & 1) {
        result *= a;
      }

      a *= a;
      n >>= 1;
    }
    return result;
  }

  static double powr(double a, size_t n) {
    if (n == 0) {
      return 1;
    }
    if (n == 1) {
      return a;
    }
    double half = powr(a, n / 2);
    if (n & 1) {
      return half * half * a;
    }
    return half * half;
  }
};