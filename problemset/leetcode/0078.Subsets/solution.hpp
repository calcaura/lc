#pragma once

#include <vector>

class Solution {
 public:
  static std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
    std::vector<std::vector<int>> result{};
    std::vector<int> prefix;

    result.push_back({});

    for (size_t idx = 0; idx < nums.size(); ++idx) {
      const size_t sz = result.size();
      for (size_t nit = 0u; nit < sz; ++nit) {
        result.push_back(result[nit]);
        result.back().push_back(nums[idx]);
      }
    }

    return result;
  }
};