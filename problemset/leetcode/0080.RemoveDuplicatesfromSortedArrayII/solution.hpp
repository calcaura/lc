#pragma once
#include <algorithm>
#include <vector>

class Solution {
 public:
  static int removeDuplicates(std::vector<int>& nums) {
    if (nums.size() <= 1) {
      return nums.size();
    }

    auto last_insert = nums.begin();
    auto search = last_insert + 1;
    const auto end = nums.end();

    for (;;) {
      if (search != end && *search == *last_insert) {
        ++last_insert;
        *last_insert = *search;
        ++search;
      }

      search = std::find_if(search, end,
                            [val = *last_insert](auto v) { return v != val; });
      if (search == end) {
        break;
      }
      const auto next_val = *search;
      ++last_insert;
      *last_insert = next_val;
      ++search;
    }

    return last_insert - nums.begin() + 1;
  }
};