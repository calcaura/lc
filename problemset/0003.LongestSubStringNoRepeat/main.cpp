
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

struct Window {
  size_t start{};
  size_t size{};
};

class Solution {
 public:
  int lengthOfLongestSubstring(const std::string& s) {
    Window max, crt;
    std::array<int, 256> syms;

    syms.fill(-1);

    for (size_t idx = 0; idx < s.length(); ++idx) {
      const char c = s[idx];
      if (syms[c] == -1) {
        ++crt.size;
        syms[c] = idx;
        continue;
      }
      // Duplicate letter. First, store into max if needed
      if (crt.size > max.size) {
        max = crt;
      }
      // Reset all chars till this position
      size_t next_index = syms[c] + 1;
      for (size_t ri = crt.start; ri < next_index; ++ri) {
        syms[s[ri]] = -1;
      }
      crt.start = next_index;
      crt.size = idx - next_index + 1;
      syms[c] = idx;
    }
    if (crt.size > max.size) {
      max = crt;
    }
    return max.size;
  }
};

int main() { return 0; }