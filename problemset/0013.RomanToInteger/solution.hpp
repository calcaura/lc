#pragma once

#include <string>

class Solution {
 public:
  int romanToInt(const std::string& s) {
    char prev{};
    int result{};

    for (char c : s) {
      switch (c) {
        case 'I':
          result += 1;
          break;
        case 'V':
          result += 5;
          if (prev == 'I') result -= 2;
          break;
        case 'X':
          result += 10;
          if (prev == 'I') result -= 2;
          break;
        case 'L':
          result += 50;
          if (prev == 'X') result -= 20;
          break;
        case 'C':
          result += 100;
          if (prev == 'X') result -= 20;
          break;
        case 'D':
          result += 500;
          if (prev == 'C') result -= 200;
          break;
        case 'M':
          result += 1000;
          if (prev == 'C') result -= 200;
          break;
        default:
          abort();
      }
      prev = c;
    }

    return result;
  }
};