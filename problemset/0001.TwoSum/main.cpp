
#include <algorithm>
#include <iostream>
#include <vector>

struct Entry {
  int value;
  int index;
  bool operator<(const Entry &v) const { return value < v.value; }

  bool operator==(const Entry &v) const { return value == v.value; }
};

class Solution {
 public:
  std::vector<int> twoSum(std::vector<int> &nums, int target) {
    std::vector<Entry> elements;
    elements.resize(nums.size());
    for (auto x = 0; x < nums.size(); ++x) {
      elements[x] = {nums[x], x};
    }

    std::sort(elements.begin(), elements.end());

    auto *last = &elements[elements.size()];

    for (auto x = &elements[0]; x != last; ++x) {
      Entry expected{target - x->value, 0};
      auto lb = std::lower_bound(x + 1, last, expected);
      if (lb != last && lb->value == expected.value) {
        return {x->index, lb->index};
      }
    }
    return {};
  }
};

int main() {
  Solution solution;
  std::vector<int> nums = {2, 7, 11, 15};
  int target = 9;
  auto result = solution.twoSum(nums, target);
  if (!result.empty()) {
    std::cout << "Indices: " << result[0] << ", " << result[1] << std::endl;
  } else {
    std::cout << "No two sum solution found." << std::endl;
  }
  return 0;
}