#pragma once
#include <algorithm>
#include <vector>

struct Entry {
  int value;
  std::size_t index;
  bool operator<(const Entry &v) const { return value < v.value; }

  bool operator==(const Entry &v) const { return value == v.value; }
};

class Solution {
 public:
  std::vector<int> twoSum(std::vector<int> &nums, int target) {
    std::vector<Entry> elements;
    elements.resize(nums.size());
    for (auto x = 0u; x < nums.size(); ++x) {
      elements[x] = {nums[x], x};
    }

    std::sort(elements.begin(), elements.end());

    auto *last = &elements[elements.size()];

    for (auto x = &elements[0]; x != last; ++x) {
      Entry expected{target - x->value, 0};
      auto lb = std::lower_bound(x + 1, last, expected);
      if (lb != last && lb->value == expected.value) {
        return {(int)x->index, (int)lb->index};
      }
    }
    return {};
  }
};
