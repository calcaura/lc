#pragma once
#include <vector>

class Solution {
 public:
  void merge(std::vector<int>& nums1, int s1, std::vector<int>& nums2, int s2) {
    auto insert = nums1.rbegin();
    auto r1 = nums1.rbegin() + (nums1.size() - s1);
    auto r2 = nums2.rbegin() + (nums2.size() - s2);

    auto e1 = nums1.rend();
    auto e2 = nums2.rend();

    while (r1 != e1 || r2 != e2) {
      if (r1 == e1) {
        for (; r2 != e2; ++r2) {
          *insert = *r2;
          ++insert;
        }
      } else if (r2 == e2) {
        for (; r1 != e1; ++r1) {
          *insert = *r1;
          ++insert;
        }
      } else {
        if (*r1 >= *r2) {
          *insert = *r1;
          ++r1;
        } else {
          *insert = *r2;
          ++r2;
        }
        ++insert;
      }
    }
  }
};