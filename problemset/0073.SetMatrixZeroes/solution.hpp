#pragma once

#include <algorithm>
#include <bitset>
#include <vector>

class Solution {
 public:
  static void setZeroes(std::vector<std::vector<int>>& matrix) {
    std::bitset<200> null_rows, null_colls;

    const size_t width = matrix[0].size();
    const size_t height = matrix.size();

    for (size_t y = 0; y < height; ++y) {
      for (size_t x = 0; x < width; ++x) {
        if (matrix[y][x] == 0) {
          null_rows[y] = 1;
          null_colls[x] = 1;
        }
      }
    }

    // nullify rows
    std::size_t popcount = null_rows.count();

    for (size_t y = 0; y < height; ++y) {
      if (null_rows[y]) {
        --popcount;
        std::ranges::fill(matrix[y], 0);
        if (popcount == 0) {
          break;
        }
      }
    }

    // nullify colls
    popcount = null_colls.count();
    for (size_t x = 0; x < width; ++x) {
      if (null_colls[x]) {
        --popcount;
        for (size_t y = 0; y < height; ++y) {
          matrix[y][x] = 0;
        }
        if (popcount == 0) {
          break;
        }
      }
    }
  }
};