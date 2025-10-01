#pragma once
#include <string_view>
#include <vector>

class Solution {
 public:
  using view = std::basic_string_view<int>;

  static int search(std::vector<int>& nums, int target) {
    auto result = search(view(&nums[0], nums.size()), target);
    if (result) {
      return result - &nums[0];
    }
    return -1;
  }

  static const int* search(view v, int target) {
    while (!v.empty()) {
      if (v.size() == 1) {
        return v[0] == target ? &v[0] : nullptr;
      }
      // Partition into two segments
      auto mid = v.size() / 2;

      // Check the current element as it'll be skipped in the partitioning
      if (target == v[mid]) {
        return &v[mid];
      }

      if (v[mid] <= v.back()) {
        // right side is sorted. Check to see if the value could be in the right
        // partition
        if (target >= v[mid] && target <= v.back()) {
          v = v.substr(mid + 1);
          continue;
        }
        // Target can't be in the right partition. Eliminate the right partition
        v = v.substr(0, mid);
      } else {
        // right is not sorted, this means left is
        if (target >= v[0] && target < v[mid]) {
          // Target could be in the left partition
          v = v.substr(0, mid);
        } else {
          // Target can't be in the left partition. Eliminate the left partition
          v = v.substr(mid + 1);
        }
      }
    }

    // Not found
    return nullptr;
  }
};