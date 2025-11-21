#pragma once

#include <cstdint>
#include <vector>

class Solution {
 public:
  static int maxPoints(std::vector<std::vector<int>>& points) {
    if (points.size() <= 2) {
      return points.size();
    }

    int max{2};
    const auto sz = points.size();

    for (size_t f_it = 0; f_it < sz; ++f_it) {
      for (size_t s_it = f_it + 1; s_it < sz; ++s_it) {
        const int px = points[f_it][0];
        const int py = points[f_it][1];

        const int64_t f_dx = px - points[s_it][0];
        const int64_t f_dy = py - points[s_it][1];

        int count{2};

        for (size_t o_it = s_it + 1; o_it < sz; ++o_it) {
          const int o_dx = px - points[o_it][0];
          const int o_dy = py - points[o_it][1];
          if (f_dy * o_dx == f_dx * o_dy) {
            ++count;
          }
        }
        if (count > max) {
          max = count;
        }
      }
    }
    return max;
  }
};