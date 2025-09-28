#pragma once

#include <array>
#include <string>

/**
 * Convert an integer to a Roman numeral using a loop-based approach.
 */
static inline std::string intToRomanLoop(int num) {
  std::string result;
  while (num) {
    if (num >= 1000) {
      result += 'M';
      num -= 1000;
      continue;
    }
    if (num >= 900) {
      result += "CM";
      num -= 900;
      continue;
    }
    if (num >= 500) {
      result += 'D';
      num -= 500;
    } else if (num >= 400) {
      result += "CD";
      num -= 400;
    } else if (num >= 100) {
      result += 'C';
      num -= 100;
    } else if (num >= 90) {
      result += "XC";
      num -= 90;
    } else if (num >= 50) {
      result += 'L';
      num -= 50;
    } else if (num >= 40) {
      result += "XL";
      num -= 40;
    } else if (num >= 10) {
      result += 'X';
      num -= 10;
    } else if (num >= 9) {
      result += "IX";
      num -= 9;
    } else if (num >= 5) {
      result += 'V';
      num -= 5;
    } else if (num >= 4) {
      result += "IV";
      num -= 4;
    } else {
      result += "I";
      num--;
    }
  }
  return result;
}

/**
 * Convert an integer to a Roman numeral using a table-based approach.
 */
static inline std::string intToRomanJT(int num) {
  static std::array<std::string_view, 4> s1000 = {"", "M", "MM", "MMM"};
  static std::array<std::string_view, 11> s100 = {
      "", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
  static std::array<std::string_view, 11> s10 = {
      "", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
  static std::array<std::string_view, 11> s1 = {
      "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X"};

  std::string result;

  result += s1000[num / 1000];
  num %= 1000;

  result += s100[num / 100];
  num %= 100;

  result += s10[num / 10];
  result += s1[num % 10];

  return result;
}

class Solution {
 public:
  std::string intToRoman(const int num) { return intToRomanJT(num); }
};